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

    // Connect signals for search and filter functionality
    connect(searchBar, &QLineEdit::textChanged, this, &Tab2::filterTable);
    connect(pollutantFilter, &QComboBox::currentIndexChanged, this, &Tab2::updateCharts);
    connect(timeRangeFilter, &QComboBox::currentIndexChanged, this, &Tab2::updateCharts);

    // Load data and populate the UI
    loadData();
    updateCharts();
}

void Tab2::createWidgets()
{
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

void Tab2::createLayout()
{
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
    QFile file("data/data.csv");
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

        if (values.size() < 5) continue;

        QString pollutant = values[3];
        if (!pollutants.contains(pollutant)) {
            pollutants.append(pollutant);
            pollutantFilter->addItem(pollutant);
        }

        csvData.append(values);
    }

    file.close();

    if (pollutants.isEmpty()) {
        qDebug() << "No pollutants found in the dataset.";
    }
}

void Tab2::updateCharts()
{
    QString selectedPollutant = pollutantFilter->currentText();
    QString selectedTimeRange = timeRangeFilter->currentText();
    QChart* chart = chartView->chart();
    chart->removeAllSeries();

    if (selectedPollutant.isEmpty()) {
        qDebug() << "No pollutant selected.";
        return;
    }

    QLineSeries* series = new QLineSeries();
    series->setName(selectedPollutant + " Levels");

    QDateTime now = QDateTime::currentDateTime();
    QDateTime startTime;

    if (selectedTimeRange == "Last Week") {
        startTime = now.addDays(-7);
    } else if (selectedTimeRange == "Last Month") {
        startTime = now.addMonths(-1);
    } else if (selectedTimeRange == "Last Year") {
        startTime = now.addYears(-1);
    }

    for (const QStringList& row : csvData) {
        if (row[3] == selectedPollutant) {
            QDateTime dateTime = QDateTime::fromString(row[0], "yyyy-MM-dd HH:mm");
            if (dateTime.isValid() && dateTime >= startTime) {
                bool ok;
                double concentration = row[4].toDouble(&ok);
                if (ok) {
                    series->append(dateTime.toMSecsSinceEpoch(), concentration);
                }
            }
        }
    }

    chart->addSeries(series);
    chart->createDefaultAxes();

    QDateTimeAxis* axisX = new QDateTimeAxis();
    axisX->setFormat("dd-MM-yyyy");
    axisX->setTitleText("Date/Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Concentration");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

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

    if (!tableModel) {
        tableModel = new QStandardItemModel(this);
    }

    tableModel->clear();
    tableModel->setHorizontalHeaderLabels({"Date/Time", "Concentration", "Compliance"});

    for (const QStringList& row : csvData) {
        if (row[3] == selectedPollutant) {
            QDateTime dateTime = QDateTime::fromString(row[0], "yyyy-MM-dd HH:mm");
            if (dateTime.isValid()) {
                bool ok;
                double concentration = row[4].toDouble(&ok);
                if (ok) {
                    QString compliance = concentration < 4 ? "Green" : (concentration < 7 ? "Amber" : "Red");

                    QList<QStandardItem*> tableRow;
                    tableRow << new QStandardItem(dateTime.toString("dd-MM-yyyy HH:mm"))
                             << new QStandardItem(QString::number(concentration))
                             << new QStandardItem(compliance);

                    tableModel->appendRow(tableRow);

                    QColor color = (compliance == "Green") ? Qt::green
                                 : (compliance == "Amber") ? Qt::yellow
                                                           : Qt::red;
                    tableModel->item(tableModel->rowCount() - 1, 2)->setBackground(color);
                }
            }
        }
    }

    dataTable->setModel(tableModel);
}

void Tab2::filterTable(const QString& searchText)
{
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

void Tab2::onPollutantSelectionChanged(int index)
{
    Q_UNUSED(index);
    updateCharts();
}

