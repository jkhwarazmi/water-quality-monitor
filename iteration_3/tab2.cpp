// Implementation for the second card - Pollutant Overview
// Nicolas

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QStandardItemModel>
#include  <QtCharts/QValueAxis>
#include <QtWidgets/QToolTip>
#include "styles.hpp"
#include "tab2.hpp"

Tab2::Tab2(WaterModel* model, QWidget* parent)
    : waterModel(model), QWidget(parent)
{
    createWidgets();
    createLayout();
    setupStyles();

    // Install event filter for theme changes
    qApp->installEventFilter(this);

    // Connect signals for search functionality
    connect(searchBar, &QLineEdit::textChanged, this, &Tab2::filterTable);

    // Connect signals to update data dynamically
    connect(pollutantFilter, &QComboBox::currentTextChanged, this, &Tab2::populateData);
    connect(timeRangeFilter, &QComboBox::currentTextChanged, this, &Tab2::populateData);

    // Initial data population
    populateData();
}

bool Tab2::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
        setupStyles();
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void Tab2::createWidgets()
{
    // Search bar
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search pollutants...");

    // Dropdown filters for pollutants and time ranges
    pollutantFilter = new QComboBox(this);
    pollutantFilter->addItems({"1,1,2-Trichloroethane", "Chloroform", "CO", "NO2", "PM2.5"});

    timeRangeFilter = new QComboBox(this);
    timeRangeFilter->addItems({"Last Week", "Last Month", "Last Year"});

    // Chart view for graphical representation
    chartView = new QChartView(new QChart(), this);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Table view for tabular data
    dataTable = new QTableView(this);

    // Placeholder label
    placeholder = new QLabel(tr("Pollutant Overview"));
    QFont labelFont;
    labelFont.setPointSize(20);
    labelFont.setBold(true);
    placeholder->setFont(labelFont);
    placeholder->setAlignment(Qt::AlignCenter);
    placeholder->setObjectName("placeholderLabel");
}

void Tab2::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;

    // Filter layout
    QHBoxLayout* filterLayout = new QHBoxLayout;
    filterLayout->addWidget(new QLabel("Pollutant:", this));
    filterLayout->addWidget(pollutantFilter);
    filterLayout->addWidget(new QLabel("Time Range:", this));
    filterLayout->addWidget(timeRangeFilter);

    layout->addWidget(searchBar); // Add search bar
    layout->addLayout(filterLayout);

    // Add chart and table views
    layout->addWidget(chartView);
    layout->addWidget(dataTable);

    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    setLayout(layout);
}

void Tab2::setupStyles()
{
    QString styleSheet = QString(R"(
        QWidget {
            background-color: transparent;
            color: %1;
        }

        QLabel#placeholderLabel {
            font-size: 20px;
            font-weight: bold;
            color: %1;
        }
    )")
    .arg(Styles::textColor());

    this->setStyleSheet(styleSheet);
}

#include <QtCharts/QValueAxis> // Include for QValueAxis
#include <QtWidgets/QToolTip>  // Include for QToolTip

void Tab2::populateData()
{
    // Populate chart with example data
    QChart* chart = chartView->chart();
    chart->removeAllSeries();

    QLineSeries* series = new QLineSeries();
    series->setName("CO Level");

    // Example data points with compliance levels
    QVector<QPointF> points = {
        {0, 0},    // Default
        {1, 3},    // Green (within compliance)
        {2, 5},    // Amber (close to threshold)
        {3, 8}     // Red (exceeds compliance)
    };

    // Set data points
    for (const QPointF& point : points) {
        series->append(point);
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Pollutant Levels Over Time");

    // Customize chart appearance
    auto* axisX = new QValueAxis(); // Ensure QValueAxis is recognized
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto* axisY = new QValueAxis(); // Ensure QValueAxis is recognized
    axisY->setTitleText("Concentration (ppm)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Add tooltips for points
    connect(series, &QLineSeries::hovered, this, [=](const QPointF& point, bool state) {
        if (state) {
            QToolTip::showText(QCursor::pos(),
                               QString("Concentration: %1 ppm\nCompliance: %2")
                                   .arg(point.y())
                                   .arg(point.y() < 4 ? "Green" : (point.y() < 7 ? "Amber" : "Red")));
        } else {
            QToolTip::hideText();
        }
    });

    // Set up table model
    if (!tableModel) {
        tableModel = new QStandardItemModel(this);
    }
    tableModel->clear();
    tableModel->setHorizontalHeaderLabels({"Date", "Pollutant", "Concentration", "Compliance"});

    // Populate table with example data
    tableModel->appendRow({new QStandardItem("2024-01-01"),
                           new QStandardItem("CO"),
                           new QStandardItem("3.5 ppm"),
                           new QStandardItem("Green")});

    tableModel->appendRow({new QStandardItem("2024-01-02"),
                           new QStandardItem("NO2"),
                           new QStandardItem("5.1 ppm"),
                           new QStandardItem("Amber")});

    tableModel->appendRow({new QStandardItem("2024-01-03"),
                           new QStandardItem("PM2.5"),
                           new QStandardItem("8.0 ppm"),
                           new QStandardItem("Red")});

    // Color code compliance column
    for (int row = 0; row < tableModel->rowCount(); ++row) {
        QString compliance = tableModel->item(row, 3)->text();
        if (compliance == "Green") {
            tableModel->item(row, 3)->setBackground(Qt::green);
        } else if (compliance == "Amber") {
            tableModel->item(row, 3)->setBackground(Qt::yellow);
        } else if (compliance == "Red") {
            tableModel->item(row, 3)->setBackground(Qt::red);
        }
    }

    dataTable->setModel(tableModel); // Set model for the table view
}

void Tab2::filterTable(const QString& searchText)
{
    if (!tableModel) return; // Ensure the model is initialized

    for (int row = 0; row < tableModel->rowCount(); ++row) {
        bool match = false;
        for (int column = 0; column < tableModel->columnCount(); ++column) {
            QStandardItem* item = tableModel->item(row, column);
            if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        dataTable->setRowHidden(row, !match);
    }
}