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

    // Connect signals for search functionality
    connect(searchBar, &QLineEdit::textChanged, this, &Tab2::filterTable);
    connect(pollutantFilter, &QComboBox::currentIndexChanged, this, &Tab2::onPollutantSelectionChanged);

    // Initialize dropdown options and populate chart
    loadData();
    updateCharts();
}

void Tab2::createWidgets()
{
    // Search bar
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search pollutants...");

    // Dropdown filter for pollutants
    pollutantFilter = new QComboBox(this);

    // Dropdown for time ranges (optional)
    timeRangeFilter = new QComboBox(this);
    timeRangeFilter->addItems({"Last Week", "Last Month", "Last Year"});

    // Chart view for graphical representation
    chartView = new QChartView(new QChart(), this);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Table view for tabular data
    dataTable = new QTableView(this);

    // Title label
    title = new QLabel(tr("Pollutant Overview"));
    QFont labelFont;
    labelFont.setPointSize(32);
    labelFont.setBold(true);
    title->setFont(labelFont);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("titleLabel");
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

    layout->addWidget(title); // Add title
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

        QLabel#titleLabel {
            font-size: 32px;
            font-weight: bold;
            color: %1;
        }
    )")
    .arg(Styles::textColor());

    this->setStyleSheet(styleSheet);
}

void Tab2::loadData()
{
    QFile file("data/data.csv"); // Assuming CSV is in the 'data' folder
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Cannot open file" << file.errorString();
        return;
    }

    QTextStream in(&file);
    bool isHeader = true;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split(',');

        if (isHeader) {
            isHeader = false; // Skip header row
            continue;
        }

        if (values.size() < 5) continue; // Ensure sufficient columns

        // Populate pollutants dropdown dynamically
        QString pollutant = values[3]; // Assuming pollutant is in column 3
        if (!pollutants.contains(pollutant)) {
            pollutants.append(pollutant);
            pollutantFilter->addItem(pollutant);
        }

        csvData.append(values); // Store the row data for later filtering
    }

    file.close();
}

void Tab2::updateCharts()
{
    QString selectedPollutant = pollutantFilter->currentText();
    QChart* chart = chartView->chart();
    chart->removeAllSeries();

    QLineSeries* series = new QLineSeries();
    series->setName(selectedPollutant + " Levels");

    // Retrieve data for selected pollutant
    const auto& dataPoints = waterModel->getPollutantData().value(selectedPollutant);
    for (const auto& point : dataPoints) {
        series->append(point.first.toMSecsSinceEpoch(), point.second);
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(tr("Levels Over Time: %1").arg(selectedPollutant));

    // Update table with data for the selected pollutant
    if (!tableModel) {
        tableModel = new QStandardItemModel(this);
    }
    tableModel->clear();
    tableModel->setHorizontalHeaderLabels({"Date/Time", "Concentration"});

    for (const auto& point : dataPoints) {
        QList<QStandardItem*> row;
        row << new QStandardItem(point.first.toString("yyyy-MM-dd HH:mm"))
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

void Tab2::onPollutantSelectionChanged(int index)
{
    Q_UNUSED(index);
    updateCharts();  // Update chart and table when pollutant is selected
}
