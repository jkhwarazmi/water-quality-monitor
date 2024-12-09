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
#include <QSet> // For handling unique pollutants
#include <QDateTime>
#include <QDebug>
#include "styles.hpp"
#include "tab2.hpp"

// Structure to hold CSV data entries
struct DataEntry {
    QString date;
    QString samplingPoint;
    QString pollutant;
    double concentration;
    QString compliance;
};

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

    // Load data from CSV
    loadDataFromCSV();

    // Populate pollutant dropdown
    populatePollutantFilter();

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

    timeRangeFilter = new QComboBox(this);
    timeRangeFilter->addItems({"All Time", "Last Week", "Last Month", "Last Year"});

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

    // Initialize data model
    tableModel = new QStandardItemModel(this);
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

void Tab2::loadDataFromCSV()
{
    // Open the CSV file
    QFile file("data.csv"); // Ensure the file path is correct
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Cannot open file" << file.errorString();
        return;
    }

    QTextStream in(&file);
    bool isHeader = true;

    dataEntries.clear();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split(',');

        if (isHeader) {
            isHeader = false; // Skip the header row
            continue;
        }

        if (values.size() < 5) continue; // Ensure sufficient columns

        // Extract data (adjust column indices based on your CSV)
        QString date = values[0];
        QString samplingPoint = values[1];
        QString pollutant = values[2];
        QString concentrationStr = values[3];
        QString compliance = values[4];

        bool ok;
        double concentration = concentrationStr.toDouble(&ok);
        if (!ok) concentration = 0.0;

        dataEntries.append({ date, samplingPoint, pollutant, concentration, compliance });
    }

    file.close();
}

void Tab2::populatePollutantFilter()
{
    // Get unique pollutants from dataEntries
    QSet<QString> pollutantsSet;
    for (const DataEntry& entry : dataEntries) {
        pollutantsSet.insert(entry.pollutant);
    }

    QStringList pollutantsList = pollutantsSet.toList();
    pollutantsList.sort();

    pollutantFilter->clear();
    pollutantFilter->addItems(pollutantsList);
}

void Tab2::populateData()
{
    QString selectedPollutant = pollutantFilter->currentText();
    QString selectedTimeRange = timeRangeFilter->currentText();

    // Filter dataEntries based on selected pollutant and time range
    QList<DataEntry> filteredData;
    QDateTime now = QDateTime::currentDateTime();

    for (const DataEntry& entry : dataEntries) {
        if (entry.pollutant != selectedPollutant)
            continue;

        // Parse date
        QDateTime entryDate = QDateTime::fromString(entry.date, Qt::ISODate);
        if (!entryDate.isValid())
            continue;

        // Apply time range filter
        if (selectedTimeRange == "Last Week" && entryDate < now.addDays(-7))
            continue;
        else if (selectedTimeRange == "Last Month" && entryDate < now.addMonths(-1))
            continue;
        else if (selectedTimeRange == "Last Year" && entryDate < now.addYears(-1))
            continue;

        filteredData.append(entry);
    }

    // Update chart
    QChart* chart = chartView->chart();
    chart->removeAllSeries();

    QLineSeries* series = new QLineSeries();
    series->setName(selectedPollutant + " Levels");

    // Sort data by date
    std::sort(filteredData.begin(), filteredData.end(), [](const DataEntry& a, const DataEntry& b) {
        return QDateTime::fromString(a.date, Qt::ISODate) < QDateTime::fromString(b.date, Qt::ISODate);
    });

    // Populate chart series
    for (const DataEntry& entry : filteredData) {
        QDateTime dateTime = QDateTime::fromString(entry.date, Qt::ISODate);
        qint64 timestamp = dateTime.toMSecsSinceEpoch();
        series->append(timestamp, entry.concentration);
    }

    chart->addSeries(series);

    // Configure axes
    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setFormat("dd MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis;
    axisY->setTitleText("Concentration");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->setTitle("Pollutant Levels Over Time (" + selectedPollutant + ")");

    // Update table
    tableModel->clear();
    tableModel->setHorizontalHeaderLabels({ "Date", "Sampling Point", "Pollutant", "Concentration", "Compliance" });

    for (const DataEntry& entry : filteredData) {
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(entry.date)
                 << new QStandardItem(entry.samplingPoint)
                 << new QStandardItem(entry.pollutant)
                 << new QStandardItem(QString::number(entry.concentration))
                 << new QStandardItem(entry.compliance);

        tableModel->appendRow(rowItems);
    }

    dataTable->setModel(tableModel);

    // Apply search filter if any
    filterTable(searchBar->text());
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
