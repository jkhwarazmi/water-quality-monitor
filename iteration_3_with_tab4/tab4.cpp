#include <QtWidgets>
#include <QtCharts>
#include "styles.hpp"
#include "tab4.hpp"

Tab4::Tab4(WaterModel* model, QWidget* parent) : waterModel(model), QWidget(parent), currentChartView(nullptr)
{
    createWidgets();
    createLayout();
    setupStyles();

    // Install event filter
    qApp->installEventFilter(this);

    // Connect signal for dropdown selection change
    connect(pollutantDropdown, &QComboBox::currentIndexChanged, this, &Tab4::onPollutantSelectionChanged);
    connect(locationDropdown, &QComboBox::currentIndexChanged, this, &Tab4::onLocationSelectionChanged);
}

void Tab4::updateTimeSeriesChart()
{
    // Deletes existing chart
    if (currentChartView) {
        QVBoxLayout* layout = static_cast<QVBoxLayout*>(this->layout());
        layout->removeWidget(currentChartView);
        delete currentChartView;
        currentChartView = nullptr;
    }

    QString pollutant = pollutantDropdown->currentText();
    QString location = locationDropdown->currentText();

    QChart* chart = createTimeSeriesChart(pollutant, location);

    if (chart) {
        // Create chart
        QChartView* chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setMinimumSize(400, 300); // Ensures Visibility

        // Adds chart to layout
        QVBoxLayout* layout = static_cast<QVBoxLayout*>(this->layout());
        layout->addWidget(chartView);

        currentChartView = chartView;

        qDebug() << "Chart successfully created and added to layout.";
    } else {
        qDebug() << "No chart created.";
    }
}

void Tab4::onPollutantSelectionChanged(int index)
{
    QString selectedPollutant = pollutantDropdown->itemText(index);
    qDebug() << "Selected pollutant:" << selectedPollutant;

    updateTimeSeriesChart();
}

void Tab4::onLocationSelectionChanged(int index)
{
    QString selectedLocation = locationDropdown->itemText(index);
    qDebug() << "Selected location:" << selectedLocation;

    updateTimeSeriesChart();
}

bool Tab4::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
        setupStyles();
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void Tab4::createWidgets()
{
    // Dropdown's labels
    pollutantLabel = new QLabel(tr("Select a Unique Pollutant:"));
    locationLabel = new QLabel(tr("Select a Unique Location:"));

    QFont labelFont;
    labelFont.setPointSize(18);
    labelFont.setBold(true);
    pollutantLabel->setFont(labelFont);
    locationLabel->setFont(labelFont);

    // Pollutants and Location dropdowns
    pollutantDropdown = new QComboBox(this);
    locationDropdown = new QComboBox(this);

    pollutantDropdown->setObjectName("pollutantDropdown");
    locationDropdown->setObjectName("locationDropdown");

    pollutantDropdown->setEditable(false);
    locationDropdown->setEditable(false);
}

void Tab4::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(pollutantLabel);
    layout->addWidget(pollutantDropdown);
    layout->addWidget(locationLabel);
    layout->addWidget(locationDropdown);
    layout->addStretch();
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    setLayout(layout);
}

void Tab4::setupStyles()
{
    QString styleSheet = QString(R"(
        QLabel {
            font-size: 18px;
            font-weight: bold;
        }

        QComboBox {
            font-size: 16px;
            padding: 5px;
        }
    )");
    this->setStyleSheet(styleSheet);
}

void Tab4::updatePollutantList()
{
    qDebug() << "Updating pollutant dropdown in Tab4...";

    if (!waterModel->hasData()) {
        pollutantDropdown->clear();
        pollutantDropdown->addItem(tr("No data available."));
        return;
    }

    const auto uniquePollutants = waterModel->getPollutantData().keys();
    pollutantDropdown->clear();

    if (uniquePollutants.isEmpty()) {
        pollutantDropdown->addItem(tr("No unique pollutants found."));
        return;
    }

    for (const auto& pollutant : uniquePollutants) {
        pollutantDropdown->addItem(pollutant);
    }
}

void Tab4::updateLocationList()
{
    qDebug() << "Updating location dropdown in Tab4...";

    if (!waterModel->hasData()) {
        locationDropdown->clear();
        locationDropdown->addItem(tr("No data available."));
        return;
    }

    const auto uniqueLocations = waterModel->getSamplingPointData().keys();
    locationDropdown->clear();

    if (uniqueLocations.isEmpty()) {
        locationDropdown->addItem(tr("No unique locations found."));
        return;
    }

    for (const auto& location : uniqueLocations) {
        locationDropdown->addItem(location);
    }
}

QChart* Tab4::createTimeSeriesChart(const QString& pollutant, const QString& location)
{
    // Both must be selected
    if (pollutant.isEmpty() || location.isEmpty()) {
        qDebug() << "Pollutant or location not selected. Cannot create chart.";
        return nullptr;
    }

    // Retrieves all matching samples
    QVector<QPair<QDateTime, double>> dataPoints;

    for (int i = 0; i < waterModel->rowCount(QModelIndex()); ++i) {
        QString samplePollutant = waterModel->data(waterModel->index(i, 4), Qt::DisplayRole).toString();
        QString sampleLocation = waterModel->data(waterModel->index(i, 2), Qt::DisplayRole).toString();

        if (samplePollutant == pollutant && sampleLocation == location) {
            QDateTime dateTime = waterModel->data(waterModel->index(i, 3), Qt::DisplayRole).toDateTime();
            double concentration = waterModel->data(waterModel->index(i, 6), Qt::DisplayRole).toDouble();
            dataPoints.append(qMakePair(dateTime, concentration));
        }
    }

    // Check if data points exist
    if (dataPoints.isEmpty()) {
        qDebug() << "No data points available for the selected pollutant and location.";
        return nullptr;
    }

    // Sort points by time
    std::sort(dataPoints.begin(), dataPoints.end(), [](const QPair<QDateTime, double>& a, const QPair<QDateTime, double>& b) {
        return a.first < b.first;
    });

    // Debug ranges
    qDebug() << "Data Points Found:" << dataPoints.size();
    qDebug() << "X-Axis Range:" << dataPoints.first().first << "to" << dataPoints.last().first;

    auto [minIt, maxIt] = std::minmax_element(dataPoints.begin(), dataPoints.end(),
        [](const QPair<QDateTime, double>& a, const QPair<QDateTime, double>& b) {
            return a.second < b.second;
        });
    qDebug() << "Y-Axis Range:" << minIt->second << "to" << maxIt->second;

    // Create series for time series chart
    QLineSeries* series = new QLineSeries();
    for (const auto& point : dataPoints) {
        qDebug() << "Appending point to series:" << point.first << point.second;
        series->append(point.first.toMSecsSinceEpoch(), point.second);
    }
    series->setName(tr("Concentration over Time"));

    // Create chart
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("Time Series for %1 at %2").arg(pollutant, location));
    chart->legend()->setVisible(true);

    // Set up X-axis (time)
    QDateTimeAxis* axisX = new QDateTimeAxis();
    axisX->setFormat("dd-MM-yyyy HH:mm");
    axisX->setTitleText(tr("Date/Time"));

    // Add padding to X-axis if only one point
    if (dataPoints.size() == 1) {
        QDateTime singlePoint = dataPoints.first().first;
        axisX->setRange(singlePoint.addSecs(-3600), singlePoint.addSecs(3600));
    } else {
        axisX->setRange(dataPoints.first().first, dataPoints.last().first);
    }
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Set up Y-axis (concentration)
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText(tr("Concentration"));

    double minY = minIt->second;
    double maxY = maxIt->second;

    // Handling for very small numbers
    if (qFuzzyCompare(minY, maxY)) {
        // All values are the same, apply relative padding
        double padding = qMax(0.1 * qAbs(minY), 1e-6);
        axisY->setRange(minY - padding, maxY + padding);
    } else {
        // Range with an extra buffer
        double padding = qMax(0.05 * (maxY - minY), 1e-6);
        axisY->setRange(minY - padding, maxY + padding);
    }

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    qDebug() << "Chart created with title:" << chart->title();
    return chart;
}
// COND @ 25 WORKS GREAT FOR TESTING
// 1,3 -DICHLOR