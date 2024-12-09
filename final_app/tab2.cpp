// Implementation for the second card - Pollutant Overview
// Nicolas

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

    // Load data and populate initial view
    loadData();
    updateTableAndChart();
}

bool Tab2::eventFilter(QObject* watched, QEvent* event)
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

void Tab2::loadData()
{
    QFile file("data.csv"); // Replace with the correct path to your data.csv file
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
            isHeader = false; // Skip the header row
            continue;
        }

        if (values.size() < 5) continue; // Ensure sufficient columns

        QString pollutant = values[3]; // Assuming pollutant label is in column 3
        if (!pollutants.contains(pollutant)) {
            pollutants.append(pollutant);
            pollutantFilter->addItem(pollutant);
        }

        csvData.append(values); // Store the entire row for filtering later
    }

    file.close();
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
    updateTableAndChart();
}
