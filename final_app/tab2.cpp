#include <QtWidgets>
#include <QtCharts>
#include "styles.hpp"
#include "tab2.hpp"

Tab2::Tab2(WaterModel* model, QWidget* parent)
    : waterModel(model), QWidget(parent) {
    createWidgets();
    createLayout();
    setupStyles();

    // Install event filter for theme changes
    qApp->installEventFilter(this);

    // Connect signals for search and filter functionality
    connect(searchBar, &QLineEdit::textChanged, this, &Tab2::filterTable);
    connect(pollutantFilter, &QComboBox::currentIndexChanged, this, &Tab2::updateCharts);
    connect(timeRangeFilter, &QComboBox::currentIndexChanged, this, &Tab2::updateCharts);

    // Populate dropdowns and update the UI
    populateDropdowns();
    updateCharts();
}

void Tab2::createWidgets() {
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search pollutants...");

    pollutantFilter = new QComboBox(this);
    timeRangeFilter = new QComboBox(this);
    timeRangeFilter->addItems({"Last Week", "Last Month", "Last Year", "All Time"});

    chartView = new QChartView(new QChart(), this);
    chartView->setRenderHint(QPainter::Antialiasing);

    dataTable = new QTableView(this);

    title = new QLabel(tr("Pollutant Overview"));
    QFont labelFont;
    labelFont.setPointSize(32);
    labelFont.setBold(true);
    title->setFont(labelFont);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("titleLabel");
}

void Tab2::createLayout() {
    QVBoxLayout* layout = new QVBoxLayout;

    QHBoxLayout* filterLayout = new QHBoxLayout;
    filterLayout->addWidget(new QLabel("Pollutant:", this));
    filterLayout->addWidget(pollutantFilter);
    filterLayout->addWidget(new QLabel("Time Range:", this));
    filterLayout->addWidget(timeRangeFilter);

    layout->addWidget(title);
    layout->addWidget(searchBar);
    layout->addLayout(filterLayout);
    layout->addWidget(chartView);
    layout->addWidget(dataTable);

    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    setLayout(layout);
}

void Tab2::setupStyles() {
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
    )").arg(Styles::textColor());

    this->setStyleSheet(styleSheet);
}

void Tab2::populateDropdowns() {
    // Retrieve pollutants from the model
    QStringList pollutants = waterModel->getPollutantList();
    pollutantFilter->clear();
    pollutantFilter->addItems(pollutants);
}

void Tab2::updateCharts() {
    QString selectedPollutant = pollutantFilter->currentText();
    QString selectedTimeRange = timeRangeFilter->currentText();

    QChart* chart = chartView->chart();
    chart->removeAllSeries();

    QLineSeries* series = new QLineSeries();
    series->setName(selectedPollutant + " Levels");

    auto dataPoints = waterModel->getTimeRangeData(selectedPollutant, selectedTimeRange);
    for (const auto& point : dataPoints) {
        series->append(point.first.toMSecsSinceEpoch(), point.second);
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(tr("Levels Over Time: %1").arg(selectedPollutant));

    auto* axisX = new QDateTimeAxis();
    axisX->setFormat("dd-MM-yyyy");
    axisX->setTitleText("Date/Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto* axisY = new QValueAxis();
    axisY->setTitleText("Concentration");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    updateTable(dataPoints);
}

void Tab2::updateTable(const QVector<QPair<QDateTime, double>>& dataPoints) {
    if (!tableModel) {
        tableModel = new QStandardItemModel(this);
    }
    tableModel->clear();
    tableModel->setHorizontalHeaderLabels({"Date/Time", "Concentration", "Compliance"});

    for (const auto& point : dataPoints) {
        QString compliance = (point.second < 4) ? "Green" : (point.second < 7) ? "Amber" : "Red";

        QList<QStandardItem*> row;
        row << new QStandardItem(point.first.toString("dd-MM-yyyy HH:mm"))
            << new QStandardItem(QString::number(point.second))
            << new QStandardItem(compliance);

        tableModel->appendRow(row);

        QColor color = (compliance == "Green") ? Qt::green
                    : (compliance == "Amber") ? Qt::yellow : Qt::red;
        tableModel->item(tableModel->rowCount() - 1, 2)->setBackground(color);
    }

    dataTable->setModel(tableModel);
}

void Tab2::filterTable(const QString& searchText) {
    if (!tableModel) return;

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
