#include <QtWidgets>
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDebug>
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

    // Connect signals for dropdown and search functionality
    connect(searchBar, &QLineEdit::textChanged, this, &Tab2::filterTable);
    connect(pollutantFilter, &QComboBox::currentIndexChanged, this, &Tab2::onPollutantSelectionChanged);


    // Initialize dropdown options and populate chart
    updateCharts();
}

void Tab2::createWidgets()
{
    // Search bar
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search pollutants...");

    // Dropdown filter for pollutants
    pollutantFilter = new QComboBox(this);

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

void Tab2::updateCharts()
{
    QString selectedPollutant = pollutantFilter->currentText();
    QChart* chart = chartView->chart();
    chart->removeAllSeries();

    QLineSeries* series = new QLineSeries();
    series->setName(selectedPollutant + " Levels");

    // Retrieve data from the model
    auto dataPoints = waterModel->getDataForPollutant(selectedPollutant);
    for (const auto& point : dataPoints) {
        series->append(point.first.toMSecsSinceEpoch(), point.second);
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(tr("Levels Over Time: %1").arg(selectedPollutant));

    // Update table data
    if (!tableModel) {
        tableModel = new QStandardItemModel(this);
    }
    tableModel->clear();
    tableModel->setHorizontalHeaderLabels({"Date/Time", "Concentration"});

    for (const auto& point : dataPoints) {
        QList<QStandardItem*> row;
        row << new QStandardItem(point.first.toString("dd-MM-yyyy"))
            << new QStandardItem(QString::number(point.second));
        tableModel->appendRow(row);
    }
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

void Tab2::updateTableAndChart()
{
    QString selectedPollutant = pollutantFilter->currentText();

    // Update table
    if (!tableModel) {
        tableModel = new QStandardItemModel(this);
    }
    tableModel->clear();
    tableModel->setHorizontalHeaderLabels({"@id", "Sampling Point", "Notation", "Label", "Concentration"});

    QChart* chart = chartView->chart();
    chart->removeAllSeries();

    QLineSeries* series = new QLineSeries();
    series->setName(selectedPollutant + " Levels");

    int row = 0;
    for (const QStringList& rowData : csvData) {
        if (rowData[3] == selectedPollutant) { // Filter rows based on the pollutant
            tableModel->appendRow({
                new QStandardItem(rowData[0]),
                new QStandardItem(rowData[1]),
                new QStandardItem(rowData[2]),
                new QStandardItem(rowData[3]),
                new QStandardItem(rowData[4])
            });

            // Add points to the chart
            bool ok;
            double concentration = rowData[4].toDouble(&ok);
            if (ok) {
                series->append(row++, concentration);
            }
        }
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Pollutant Levels Over Time (" + selectedPollutant + ")");

    dataTable->setModel(tableModel);
}


void Tab2::onPollutantSelectionChanged(int index)
{
    Q_UNUSED(index);
    updateTableAndChart();
}

