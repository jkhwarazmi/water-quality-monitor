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
#include <QtCharts/QValueAxis>
#include <QtWidgets/QToolTip>
#include <QFile>
#include <QTextStream>
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

void Tab2::populateData()
{
    // Open the CSV file
    QFile file("data.csv"); // Replace with the correct path to your data.csv file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Cannot open file" << file.errorString();
        return;
    }

    QChart* chart = chartView->chart();
    chart->removeAllSeries();

    QLineSeries* series = new QLineSeries();
    series->setName("Pollutant Levels");

    if (!tableModel) {
        tableModel = new QStandardItemModel(this);
    }
    tableModel->clear();
    tableModel->setHorizontalHeaderLabels({"@id", "Sampling Point", "Notation", "Label"}); // Adjust headers

    QTextStream in(&file);
    bool isHeader = true;
    int row = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split(',');

        if (isHeader) {
            isHeader = false; // Skip the header row
            continue;
        }

        if (values.size() < 4) continue; // Ensure sufficient columns

        // Extract data (adjust column indices as needed)
        QString id = values[0];
        QString samplingPoint = values[1];
        QString notation = values[2];
        QString label = values[3];

        // Add row to the table
        tableModel->appendRow({
            new QStandardItem(id),
            new QStandardItem(samplingPoint),
            new QStandardItem(notation),
            new QStandardItem(label)
        });

        // Populate chart (example using notation as a numeric value)
        bool ok;
        double value = notation.toDouble(&ok); // Adjust column index
        if (ok) {
            series->append(row++, value);
        }
    }

    file.close();

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Pollutant Levels Over Time");

    dataTable->setModel(tableModel);
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
