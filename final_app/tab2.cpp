#include <QtWidgets>
#include <QtCharts>
#include "styles.hpp"
#include "tab2.hpp"

Tab2::Tab2(WaterModel* model, QWidget* parent) : waterModel(model), QWidget(parent), currentChartView(nullptr)
{
    createWidgets();
    createLayout();
    setupStyles();

    // Install event filter
    qApp->installEventFilter(this);

    // Connect signal for dropdown selection change
    connect(pollutantDropdown, &QComboBox::currentIndexChanged, this, &Tab2::onPollutantSelectionChanged);
    connect(locationDropdown, &QComboBox::currentIndexChanged, this, &Tab2::onLocationSelectionChanged);
}

void Tab2::updateLocationDropdown(const QString& pollutant)
{
    // Block signals to prevent unwanted slot executions during update
    locationDropdown->blockSignals(true);
    locationDropdown->clear();

    if (pollutant.isEmpty() || pollutant == tr("No unique pollutants found.") || pollutant == tr("No data available.")) {
        locationDropdown->addItem(tr("No data available."));
        locationDropdown->blockSignals(false);
        return;
    }

    // Retrieve the set of locations for the selected pollutant from the model
    QSet<QString> locations = waterModel->getPollutantToLocations().value(pollutant);

    if (locations.isEmpty()) {
        locationDropdown->addItem(tr("No locations found for this pollutant."));
    } else {
        // Convert QSet to a sorted QStringList for better UX
        QStringList sortedLocations = locations.values();
        sortedLocations.sort();
        for (const QString& location : sortedLocations) {
            locationDropdown->addItem(location);
        }
    }

    locationDropdown->blockSignals(false);

    // Optionally, trigger an update to the chart if locations are available
    if (!locations.isEmpty()) {
        // Select the first location by default
        locationDropdown->setCurrentIndex(0);
        updateTimeSeriesChart();
    } else {
        // Clear the chart if no locations are available
        if (currentChartView) {
            QVBoxLayout* layout = static_cast<QVBoxLayout*>(this->layout());
            layout->removeWidget(currentChartView);
            delete currentChartView;
            currentChartView = nullptr;
        }
    }
}


void Tab2::updateTimeSeriesChart()
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

void Tab2::onPollutantSelectionChanged(int index)
{
    QString selectedPollutant = pollutantDropdown->itemText(index);
    qDebug() << "Selected pollutant:" << selectedPollutant;

    // Update the location dropdown based on the selected pollutant
    updateLocationDropdown(selectedPollutant);
}


void Tab2::onLocationSelectionChanged(int index)
{
    QString selectedLocation = locationDropdown->itemText(index);
    qDebug() << "Selected location:" << selectedLocation;

    updateTimeSeriesChart();
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
    title = new QLabel(tr("Fluorinated Compounds"));
    QFont titleFont;
    titleFont.setPointSize(32);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("titleLabel");

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

void Tab2::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addWidget(pollutantLabel);
    layout->addWidget(pollutantDropdown);
    layout->addWidget(locationLabel);
    layout->addWidget(locationDropdown);
    layout->addStretch();
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    setLayout(layout);
}

void Tab2::setupStyles()
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

void Tab2::updateCharts()
{
    updatePollutantList();
    updateLocationList();
}

void Tab2::updatePollutantList()
{
    qDebug() << "Updating pollutant dropdown in Tab2...";

    pollutantDropdown->blockSignals(true); // Prevent triggering slot during update
    pollutantDropdown->clear();

    if (!waterModel->hasData()) {
        pollutantDropdown->addItem(tr("No data available."));
        pollutantDropdown->blockSignals(false);
        updateLocationDropdown(QString());
        return;
    }

    const auto uniquePollutants = waterModel->getFluorPollutantData().keys();
    pollutantDropdown->clear();

    if (uniquePollutants.isEmpty()) {
        pollutantDropdown->addItem(tr("No unique pollutants found."));
        pollutantDropdown->blockSignals(false);
        updateLocationDropdown(QString());
        return;
    }

    // Populate the pollutant dropdown
    for (const auto& pollutant : uniquePollutants) {
        pollutantDropdown->addItem(pollutant);
        qDebug() << pollutant;
    }

    pollutantDropdown->blockSignals(false);

    // Optionally, select the first pollutant and update locations
    if (!uniquePollutants.isEmpty()) {
        pollutantDropdown->setCurrentIndex(0);
        updateLocationDropdown(pollutantDropdown->currentText());
    }
}


void Tab2::updateLocationList()
{
    qDebug() << "Updating location dropdown in Tab2...";

    if (!waterModel->hasData()) {
        locationDropdown->clear();
        locationDropdown->addItem(tr("No data available."));
        return;
    }

    const auto uniqueLocations = waterModel->getFluorLocations().keys();
    locationDropdown->clear();

    if (uniqueLocations.isEmpty()) {
        locationDropdown->addItem(tr("No unique locations found."));
        return;
    }

    for (const auto& location : uniqueLocations) {
        locationDropdown->addItem(location);
    }
}

QChart* Tab2::createTimeSeriesChart(const QString& pollutant, const QString& location)
{
    // Validate selections
    if (pollutant.isEmpty() || location.isEmpty()) {
        qDebug() << "Pollutant or location not selected. Cannot create chart.";
        return nullptr;
    }

    // Retrieve all matching samples
    QVector<QPair<QDateTime, double>> dataPoints;

    for (int i = 0; i < waterModel->rowCount(QModelIndex()); ++i) {
        QString samplePollutant = waterModel->data(waterModel->index(i, 4), Qt::DisplayRole).toString();
        QString sampleLocation = waterModel->data(waterModel->index(i, 2), Qt::DisplayRole).toString();

        if (samplePollutant == pollutant && sampleLocation == location) {
            QDateTime dateTime = waterModel->data(waterModel->index(i, 3), Qt::DisplayRole).toDateTime();
            double concentration = waterModel->data(waterModel->index(i, 6), Qt::DisplayRole).toDouble();

            // Validate data points
            if (!dateTime.isValid()) {
                qDebug() << "Invalid QDateTime at row" << i << ". Skipping this data point.";
                continue;
            }

            if (qIsNaN(concentration)) {
                qDebug() << "NaN concentration at row" << i << ". Skipping this data point.";
                continue;
            }

            dataPoints.append(qMakePair(dateTime, concentration));
        }
    }

    qDebug() << "Total valid data points retrieved:" << dataPoints.size();

    // Check if data points exist
    if (dataPoints.isEmpty()) {
        qDebug() << "No valid data points available for the selected pollutant and location.";
        return nullptr;
    }

    // Sort points by time
    std::sort(dataPoints.begin(), dataPoints.end(), [](const QPair<QDateTime, double>& a, const QPair<QDateTime, double>& b) {
        return a.first < b.first;
    });

    // Retrieve threshold
    double threshold = waterModel->getDeterminandThreshold(pollutant);
    qDebug() << "Threshold for" << pollutant << ":" << threshold;

    if (threshold <= 0.0) {
        qDebug() << "Invalid threshold for" << pollutant << ". Threshold must be greater than 0.";
        return nullptr;
    }

    // Create line series
    QLineSeries* lineSeries = new QLineSeries();
    lineSeries->setName(tr("Concentration over Time"));
    lineSeries->setColor(QColor(0, 0, 255)); // Blue line for trend

    // Create scatter series for each compliance cat
    QScatterSeries* compliantSeries = new QScatterSeries();
    compliantSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    compliantSeries->setMarkerSize(10.0);
    compliantSeries->setName(tr("Compliant"));
    compliantSeries->setBrush(QBrush(QColor(11, 102, 35))); // Green
    compliantSeries->setPen(QPen(QColor(11, 102, 35)));

    QScatterSeries* nearThresholdSeries = new QScatterSeries();
    nearThresholdSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    nearThresholdSeries->setMarkerSize(10.0);
    nearThresholdSeries->setName(tr("Near Threshold"));
    nearThresholdSeries->setBrush(QBrush(QColor(255, 191, 0))); // Amber
    nearThresholdSeries->setPen(QPen(QColor(255, 191, 0)));

    QScatterSeries* nonCompliantSeries = new QScatterSeries();
    nonCompliantSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    nonCompliantSeries->setMarkerSize(10.0);
    nonCompliantSeries->setName(tr("Non-Compliant"));
    nonCompliantSeries->setBrush(QBrush(QColor(215, 0, 64))); // Rou
    nonCompliantSeries->setPen(QPen(QColor(215, 0, 64)));

    // category counters
    int compliantCount = 0;
    int nearThresholdCount = 0;
    int nonCompliantCount = 0;

    // Implement data points into series
    for (const auto& point : dataPoints) {
        qint64 x = point.first.toMSecsSinceEpoch(); // xaxis as milliseconds
        double y = point.second; // y concentration acis

        // appends to line series
        lineSeries->append(x, y);

        // determine status and add to series
        if (y <= threshold * 0.8) {
            compliantSeries->append(x, y);
            compliantCount++;
        }
        else if (y > threshold * 0.8 && y <= threshold) {
            nearThresholdSeries->append(x, y);
            nearThresholdCount++;
        }
        else {
            nonCompliantSeries->append(x, y);
            nonCompliantCount++;
        }
    }

    qDebug() << "Compliant points:" << compliantCount;
    qDebug() << "Near Threshold points:" << nearThresholdCount;
    qDebug() << "Non-Compliant points:" << nonCompliantCount;

    // creat chart, add series
    QChart* chart = new QChart();
    chart->addSeries(lineSeries);

    // Only adds scatter series only if they have data points
    if (compliantSeries->count() > 0) {
        chart->addSeries(compliantSeries);
    } else {
        qDebug() << "No compliant data points to display.";
    }

    if (nearThresholdSeries->count() > 0) {
        chart->addSeries(nearThresholdSeries);
    } else {
        qDebug() << "No near threshold data points to display.";
    }

    if (nonCompliantSeries->count() > 0) {
        chart->addSeries(nonCompliantSeries);
    } else {
        qDebug() << "No non-compliant data points to display.";
    }

    chart->setTitle(tr("Time Series for %1 at %2").arg(pollutant, location));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Displays tooltips when hovered is detected
    auto setupTooltip = [&](QScatterSeries* series, const QString& status) {
        connect(series, &QScatterSeries::hovered, this, [&, status](const QPointF &point, bool state) {
            if (state) {
                // converts x to datetime
                QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(point.x()));
                double concentration = point.y();

                QString tooltipText = tr("Date Time: %1\nConcentration: %2 µg/L\nStatus: %3")
                    .arg(dateTime.toString("dd-MM-yyyy HH:mm"))
                    .arg(concentration)
                    .arg(status);

                QToolTip::showText(QCursor::pos(), tooltipText, this);
            }
            else {
                QToolTip::hideText();
            }
        });
    };

    if (compliantCount > 0)
        setupTooltip(compliantSeries, tr("Compliant"));
    if (nearThresholdCount > 0)
        setupTooltip(nearThresholdSeries, tr("Near Threshold"));
    if (nonCompliantCount > 0)
        setupTooltip(nonCompliantSeries, tr("Non-Compliant"));

    // Sets up the x axis (time)
    QDateTimeAxis* axisX = new QDateTimeAxis();
    axisX->setFormat("dd-MM-yyyy HH:mm");
    axisX->setTitleText(tr("Date/Time"));

    // defines range of xaxis
    if (dataPoints.size() == 1) {
        // SingplePoint add padding
        QDateTime singlePoint = dataPoints.first().first;
        axisX->setRange(singlePoint.addSecs(-3600), singlePoint.addSecs(3600));
    }
    else {
        axisX->setRange(dataPoints.first().first, dataPoints.last().first);
    }

    chart->addAxis(axisX, Qt::AlignBottom);
    lineSeries->attachAxis(axisX);
    if (compliantCount > 0)
        compliantSeries->attachAxis(axisX);
    if (nearThresholdCount > 0)
        nearThresholdSeries->attachAxis(axisX);
    if (nonCompliantCount > 0)
        nonCompliantSeries->attachAxis(axisX);

    // sets up concentration y-axis
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText(tr("Concentration (µg/L)"));

    double minY = std::min_element(dataPoints.begin(), dataPoints.end(),
        [](const QPair<QDateTime, double>& a, const QPair<QDateTime, double>& b) {
            return a.second < b.second;
        })->second;

    double maxY = std::max_element(dataPoints.begin(), dataPoints.end(),
        [](const QPair<QDateTime, double>& a, const QPair<QDateTime, double>& b) {
            return a.second < b.second;
        })->second;

    // Handle case where all y-values are the same
    if (qFuzzyCompare(minY, maxY)) {
        double padding = qMax(0.1 * qAbs(minY), 1e-6);
        axisY->setRange(minY - padding, maxY + padding);
    }
    else {
        double padding = qMax(0.05 * (maxY - minY), 1e-6);
        axisY->setRange(minY - padding, maxY + padding);
    }

    chart->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisY);
    if (compliantCount > 0)
        compliantSeries->attachAxis(axisY);
    if (nearThresholdCount > 0)
        nearThresholdSeries->attachAxis(axisY);
    if (nonCompliantCount > 0)
        nonCompliantSeries->attachAxis(axisY);

    // adding horizontal line for threshold
    QLineSeries* thresholdLine = new QLineSeries();
    thresholdLine->append(axisX->min().toMSecsSinceEpoch(), threshold);
    thresholdLine->append(axisX->max().toMSecsSinceEpoch(), threshold);
    thresholdLine->setName(tr("Threshold"));
    thresholdLine->setColor(QColor(128, 128, 128)); // GREY
    thresholdLine->setPen(QPen(QColor(254, 0, 0), 2, Qt::DashLine)); // DASHED RED
    chart->addSeries(thresholdLine);
    thresholdLine->attachAxis(axisX);
    thresholdLine->attachAxis(axisY);

    // ensures threshold line is on top
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    qDebug() << "Chart successfully created with title:" << chart->title();
    qDebug() << "Chart has" << chart->series().size() << "series.";

    return chart;
}
// COND @ 25 WORKS GREAT FOR TESTING
// 1,3 -DICHLOR
// PFOS WORKS FAB TOO