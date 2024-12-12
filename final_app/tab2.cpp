// Implementation for the second card - Pollutant Overview
// Nicolas

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

    // Connect signals for dropdown selection changes
    connect(pollutantDropdown, &QComboBox::currentIndexChanged, this, &Tab2::onPollutantSelectionChanged);
    connect(locationDropdown, &QComboBox::currentIndexChanged, this, &Tab2::onLocationSelectionChanged);

    // Initialise the dropdowns and chart
    updateCharts();
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
        chartView->setMinimumSize(400, 300);

        // Adds chart to layout
        QVBoxLayout* layout = static_cast<QVBoxLayout*>(this->layout());
        layout->addWidget(chartView);

        currentChartView = chartView;
    }
}

void Tab2::onPollutantSelectionChanged(int index)
{
    QString selectedPollutant = pollutantDropdown->itemText(index);

    // Update the location dropdown based on the selected pollutant
    updateLocationDropdown(selectedPollutant);
}


void Tab2::onLocationSelectionChanged(int index)
{
    QString selectedLocation = locationDropdown->itemText(index);

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
    title = new QLabel(tr("Pollutant Overview"));
    QFont titleFont;
    titleFont.setPointSize(32);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("titleLabel");

    description = new QLabel(tr("Monitor pollutant concentrations in water samples over time. "
                               "Select a pollutant and location to view detailed time series data with compliance indicators."));
    description->setWordWrap(true);
    description->setAlignment(Qt::AlignCenter);
    description->setObjectName("descriptionLabel");
    QFont descriptionFont;
    descriptionFont.setPointSize(14);
    description->setFont(descriptionFont);

    // Dropdown labels with consistent styling
    pollutantLabel = new QLabel(tr("Select Pollutant:"));
    locationLabel = new QLabel(tr("Select Location:"));

    QFont labelFont;
    labelFont.setPointSize(14);
    labelFont.setBold(true);
    pollutantLabel->setFont(labelFont);
    locationLabel->setFont(labelFont);

    // Style the dropdowns
    pollutantDropdown = new QComboBox(this);
    locationDropdown = new QComboBox(this);

    pollutantDropdown->setObjectName("dataDropdown");
    locationDropdown->setObjectName("dataDropdown");

    pollutantDropdown->setFixedHeight(40);
    locationDropdown->setFixedHeight(40);
    
    pollutantDropdown->setEditable(false);
    locationDropdown->setEditable(false);
}

void Tab2::createLayout()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    // Title section with consistent spacing
    mainLayout->addWidget(title);
    mainLayout->addWidget(description);
    mainLayout->addSpacing(20);

    // Dropdown section in horizontal layout
    QHBoxLayout* dropdownLayout = new QHBoxLayout;
    
    QVBoxLayout* pollutantSection = new QVBoxLayout;
    pollutantSection->addWidget(pollutantLabel);
    pollutantSection->addWidget(pollutantDropdown);
    
    QVBoxLayout* locationSection = new QVBoxLayout;
    locationSection->addWidget(locationLabel);
    locationSection->addWidget(locationDropdown);
    
    dropdownLayout->addLayout(pollutantSection);
    dropdownLayout->addLayout(locationSection);
    
    mainLayout->addLayout(dropdownLayout);
    mainLayout->addSpacing(20);
    
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    setLayout(mainLayout);
}

void Tab2::setupGeneralStyles()
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

        QLabel#descriptionLabel {
            color: %1;
            margin-bottom: 20px;
        }
    )")
    .arg(Styles::textColor());

    this->setStyleSheet(this->styleSheet() + styleSheet);
}

void Tab2::setupComboBoxStyles()
{
    QString styleSheet = QString(R"(
        QComboBox {
            font-size: 16px;
            padding: 5px;
            background-color: %2;  /* Dropdown background color */
            color: %1;  /* Text color in dropdown */
            border: 1px solid %3;  /* Border color */
        })")
    .arg(Styles::textColor())  // Text color
    .arg(Styles::backgroundColor())  // Dropdown background color
    .arg(Styles::borderColor());  // Border color for dropdown

    this->setStyleSheet(this->styleSheet() + styleSheet);
}

void Tab2::setupStyles()
{
    setupGeneralStyles();
    setupComboBoxStyles();
}

void Tab2::updateCharts()
{
    // Block signals to prevent unwanted updates during initialisation
    pollutantDropdown->blockSignals(true);
    locationDropdown->blockSignals(true);

    pollutantDropdown->clear();
    QSet<QString> uniquePollutants;
    
    // Get all pollutants from the model
    const auto& pollutantData = waterModel->getPollutantToLocations();
    if (pollutantData.isEmpty()) {
        pollutantDropdown->addItem(tr("No data available."));
        pollutantDropdown->blockSignals(false);
        locationDropdown->blockSignals(false);
        return;
    }

    // Add all pollutants to the dropdown
    for (auto it = pollutantData.begin(); it != pollutantData.end(); ++it) {
        uniquePollutants.insert(it.key());
    }

    if (uniquePollutants.isEmpty()) {
        pollutantDropdown->addItem(tr("No unique pollutants found."));
    } else {
        // Convert to sorted list for better UX
        QStringList sortedPollutants = uniquePollutants.values();
        sortedPollutants.sort();
        pollutantDropdown->addItems(sortedPollutants);
        
        // Select first pollutant
        pollutantDropdown->setCurrentIndex(0);
        QString selectedPollutant = pollutantDropdown->currentText();
        
        // Update location dropdown for the selected pollutant
        updateLocationDropdown(selectedPollutant);
        
        // Select first location
        if (locationDropdown->count() > 0) {
            locationDropdown->setCurrentIndex(0);
        }
    }

    pollutantDropdown->blockSignals(false);
    locationDropdown->blockSignals(false);
}

QChart* Tab2::createTimeSeriesChart(const QString& pollutant, const QString& location)
{
    // Validate selections
    if (pollutant.isEmpty() || location.isEmpty()) {
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
                continue;
            }

            if (qIsNaN(concentration)) {
                continue;
            }

            dataPoints.append(qMakePair(dateTime, concentration));
        }
    }
    // Check if data points exist
    if (dataPoints.isEmpty()) {
        return nullptr;
    }

    // Sort points by time
    std::sort(dataPoints.begin(), dataPoints.end(), [](const QPair<QDateTime, double>& a, const QPair<QDateTime, double>& b) {
        return a.first < b.first;
    });

    // Retrieve threshold
    double threshold = waterModel->getDeterminandThreshold(pollutant);

    if (threshold <= 0.0) {
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
    nonCompliantSeries->setBrush(QBrush(QColor(215, 0, 64))); // Red
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

    // creat chart, add series
    QChart* chart = new QChart();
    chart->addSeries(lineSeries);

    // Only adds scatter series only if they have data points
    if (compliantSeries->count() > 0) {
        chart->addSeries(compliantSeries);
    } 

    if (nearThresholdSeries->count() > 0) {
        chart->addSeries(nearThresholdSeries);
    }

    if (nonCompliantSeries->count() > 0) {
        chart->addSeries(nonCompliantSeries);
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

    return chart;
}
