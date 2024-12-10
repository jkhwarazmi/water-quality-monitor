#include <QtWidgets>
// #include <QtCharts>
#include <QHBoxLayout>
#include <QFrame>
#include "styles.hpp"
#include "tab5.hpp"
#include "model.hpp"


Tab5::Tab5(WaterModel* model, QWidget* parent) : QWidget(parent), waterModel(model), complianceLayout(new QHBoxLayout)
//, currentChartView(nullptr)
{
    createWidgets();
    // createCharts();
    setupStyles();
    createLayout();

    // Install event filter
    qApp->installEventFilter(this);

    // Connect signal for dropdown selection change
    connect(pollutantDropdown, &QComboBox::currentIndexChanged, this, &Tab5::onPollutantSelectionChanged);
    connect(locationDropdown, &QComboBox::currentIndexChanged, this, &Tab5::onLocationSelectionChanged);
    //connect(pollutantDropdown, &QComboBox::currentIndexChanged, this, &Tab5::updateBarChart);
}



void Tab5::onPollutantSelectionChanged(int index)
{
    QString selectedPollutant = pollutantDropdown->itemText(index);
    updateComplianceStatus(selectedPollutant, locationDropdown->currentText());
}

void Tab5::onLocationSelectionChanged(int index)
{
    QString selectedLocation = locationDropdown->itemText(index);
    QString selectedPollutant = pollutantDropdown->currentText(); // Add this line to get the selected pollutant

    // Now you can use selectedPollutant in the updateComplianceStatus call
    updateComplianceStatus(selectedPollutant, selectedLocation);
}

bool Tab5::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
        setupStyles();
        return true;
    }
    return QWidget::eventFilter(watched, event);
}



void Tab5::createWidgets()
{
    title = new QLabel(tr("Compliance Information"));
    QFont titleFont;
    titleFont.setPointSize(32);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("titleLabel");

    // Dropdown's labels
    pollutantLabel = new QLabel(tr("Select a Unique Pollutant:"));
    locationLabel = new QLabel(tr("Select a Unique Location:"));

    // Function - New
    definitionLabel = new QLabel(tr("Definition: Not available"));
    thresholdLabel = new QLabel(tr("EU/UK Standard: Not available"));
    currentValueLabel = new QLabel(tr("Current Value: Not available"));

    QFont labelFont;
    labelFont.setPointSize(18);
    labelFont.setBold(true);
    pollutantLabel->setFont(labelFont);
    locationLabel->setFont(labelFont);
    definitionLabel->setFont(labelFont);
    thresholdLabel->setFont(labelFont);
    currentValueLabel->setFont(labelFont);

    // Pollutants and Location dropdowns
    pollutantDropdown = new QComboBox(this);
    locationDropdown = new QComboBox(this);

    pollutantDropdown->setObjectName("pollutantDropdown");
    locationDropdown->setObjectName("locationDropdown");

    pollutantDropdown->setEditable(false);
    locationDropdown->setEditable(false);

    // Compliance Widgets
    complianceStatusLabel = new QLabel(tr("Compliance Status:"));
    complianceStatusLabel->setAlignment(Qt::AlignCenter);

    trafficLightWidget = new QWidget(this);
    trafficLightWidget->setFixedSize(50, 50);

    // Compliance Summary Card
    complianceCard = new QFrame(this);
    complianceCard->setObjectName("complianceCard");
    complianceCard->setFrameShape(QFrame::StyledPanel);
    complianceCard->setFrameShadow(QFrame::Raised);

    // Set the size policy to allow dynamic resizing
    complianceCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Optionally set a minimum and maximum size
    complianceCard->setMinimumSize(400, 400);  // Minimum size: 400x300px
    complianceCard->setMaximumSize(800, 800);  // Maximum size: 800x600px

    // Layout for the compliance card content
    QVBoxLayout* cardLayout = new QVBoxLayout(complianceCard);

    // Add widgets to the card layout in the desired order
    cardLayout->addWidget(definitionLabel, 0, Qt::AlignLeft);
    cardLayout->addWidget(thresholdLabel, 0, Qt::AlignLeft);
    cardLayout->addWidget(currentValueLabel, 0, Qt::AlignLeft);

    // Legend section
    QLabel* legendLabel = new QLabel(tr("Legend:"));
    QVBoxLayout* legendLayout = new QVBoxLayout;
    QLabel* greenLegend = new QLabel(tr("• Green: Compliant"));
    greenLegend->setStyleSheet("color: green;");
    QLabel* amberLegend = new QLabel(tr("• yellow: Near Threshold"));
    amberLegend->setStyleSheet("color: yellow;");
    QLabel* redLegend = new QLabel(tr("• Red: Non-Compliant"));
    redLegend->setStyleSheet("color: red;");

    legendLayout->addWidget(legendLabel);
    legendLayout->addWidget(greenLegend);
    legendLayout->addWidget(amberLegend);
    legendLayout->addWidget(redLegend);

    cardLayout->addLayout(legendLayout); // Add legend layout to the card layout

    // Compliance status and traffic light section
    cardLayout->addWidget(complianceStatusLabel, 0, Qt::AlignCenter);

    // Center the traffic light widget
    QHBoxLayout* trafficLightLayout = new QHBoxLayout;
    trafficLightLayout->addStretch();
    trafficLightLayout->addWidget(trafficLightWidget);
    trafficLightLayout->addStretch();
    cardLayout->addLayout(trafficLightLayout);

    // Set layout for the compliance card
    complianceCard->setLayout(cardLayout);
}

//Modular Layout

void Tab5::createLayout()
{
    // Main layout for Tab5
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(title);

    // Add pollutant and location dropdowns
    mainLayout->addWidget(pollutantLabel);
    mainLayout->addWidget(pollutantDropdown);
    mainLayout->addWidget(locationLabel);
    mainLayout->addWidget(locationDropdown);

    // Add the compliance card directly below the dropdowns
    mainLayout->addWidget(complianceCard);

    // Chart section and additional layout (currently empty)
    QVBoxLayout* chartLayout = new QVBoxLayout;
    mainLayout->addLayout(chartLayout);

    // Add the bar chart view if it exists
    if (chartView) {
        mainLayout->addWidget(chartView);
    }

    // Add stretch to fill the remaining space
    mainLayout->addStretch();

    // Set margins and spacing
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Set the layout for the widget
    setLayout(mainLayout);
}



void Tab5::setupStyles()
{
    QString styleSheet = QString(R"(
        QLabel {
            font-size: 18px;
            
        }

        QComboBox {
            font-size: 16px;
            padding: 5px;
        }

        QWidget#Tab5 {
            border: 2px solid white;
            border-radius: 8px;
            background-color: #f9f9f9;
        }

        QFrame#complianceCard {
            background-color: white;
            border: 2px solid #dcdcdc;
            border-radius: 10px;
            padding: 15px;
        }

        QWidget#trafficLightWidget {
            border-radius: 25px;  // Circle
        }
    )");
    this->setObjectName("Tab5");
    this->setStyleSheet(styleSheet);
}

void Tab5::updateCharts()
{
    updatePollutantList();
    updateLocationList();
}

void Tab5::updatePollutantList()
{
    // Define the list of pollutants you want to select from the model
    QStringList selectedPollutants = {
    "Ammonia(N)", 
    "BOD ATU", 
    "Sld Sus@105C", 
    "O Diss %sat", 
    "Orthophospht", 
    "N Oxidised", 
    "Nitrite-N", 
    "Nitrate-N", 
    "NH3 un-ion", 
    "COD as O2", 
    "Phosphorus-P", 
    "Nitrogen - N"
    }; // Customize this list

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

    // Filter pollutants to only include the selected ones
    for (const auto& pollutant : uniquePollutants) {
        if (selectedPollutants.contains(pollutant)) {
            pollutantDropdown->addItem(pollutant);
        }
    }

    // If no pollutants match, you can add a default message
    if (pollutantDropdown->count() == 0) {
        pollutantDropdown->addItem(tr("No matching pollutants found."));
    }
}

void Tab5::updateLocationList()
{
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

//NEW
void Tab5::updateComplianceStatus(const QString& pollutant, const QString& location)
{
    if (pollutant.isEmpty() || location.isEmpty()) {
        complianceStatusLabel->setText(tr("Please select both a pollutant and a location."));
        definitionLabel->setText(tr("Definition: Not available"));
        thresholdLabel->setText(tr("EU/UK Standard: Not available"));
        currentValueLabel->setText(tr("Current Value: Not available"));
        trafficLightWidget->setStyleSheet("background-color: black;");
        return;
    }

    // Retrieve pollutant threshold from WaterModel
    double threshold = waterModel->getDeterminandThreshold(pollutant);
    QString definition = waterModel->getDeterminandDefinition(pollutant);
    if (threshold == 0.0) {
        definitionLabel->setText(tr("Definition: %1").arg(definition.isEmpty() ? tr("Not available") : definition));
        complianceStatusLabel->setText(tr("No threshold defined for this pollutant."));
        thresholdLabel->setText(tr("EU/UK Standard: Not available"));
        currentValueLabel->setText(tr("Current Value: Not available"));
        trafficLightWidget->setStyleSheet("background-color: black;");
        return;
    }
    //funtion-new

     thresholdLabel->setText(tr("EU/UK Standard: %1").arg(threshold));
     definitionLabel->setText(tr("Definition: %1").arg(definition.isEmpty() ? tr("Not available") : definition));


    // Check if the pollutant concentration at the location exceeds the threshold
    bool isCompliant = false;
    QVector<QPair<QDateTime, double>> dataPoints;

    double concentration = -1.0; // Initialize with an invalid value
    for (int i = 0; i < waterModel->rowCount(QModelIndex()); ++i) {
        QString samplePollutant = waterModel->data(waterModel->index(i, 4), Qt::DisplayRole).toString();
        QString sampleLocation = waterModel->data(waterModel->index(i, 2), Qt::DisplayRole).toString();

        if (samplePollutant == pollutant && sampleLocation == location) {
            concentration = waterModel->data(waterModel->index(i, 6), Qt::DisplayRole).toDouble();
            break; // Use the first matching value
        }
    }

    // Update compliance status and traffic light color
    if (concentration < 0.0) {
        complianceStatusLabel->setText(tr("No data available for the selected location and pollutant."));
        currentValueLabel->setText(tr("Current Value: Not available"));
        trafficLightWidget->setStyleSheet("background-color: black;");
        return;
    }

    // Update Current Value label
    currentValueLabel->setText(tr("Current Value: %1").arg(concentration));

    // Determine and display compliance status
if (concentration <= threshold) {
    // Fully compliant, concentration is less than or equal to threshold
    complianceStatusLabel->setText(tr("Compliant"));
    trafficLightWidget->setStyleSheet("background-color: green;");
} else if (concentration <= threshold * 0.8) {
    // Amber color when concentration is near the threshold (within 80% of threshold)
    complianceStatusLabel->setText(tr("Near Threshold"));
    trafficLightWidget->setStyleSheet("background-color: yellow;");
} else {
    // Non-compliant, concentration is above 80% of the threshold
    complianceStatusLabel->setText(tr("Non-Compliant"));
    trafficLightWidget->setStyleSheet("background-color: red;");
}

}

// void Tab5::updateBarChart()
// {
//     // Ensure the pollutantDropdown is the correct widget used for selection
//     QString selectedPollutant = pollutantDropdown->currentText(); // Get selected pollutant

//     if (selectedPollutant.isEmpty()) {
//         qDebug() << "No pollutant selected.";
//         return; // Exit if no pollutant is selected
//     }

//     // Fetch the top 5 locations by concentration for the selected pollutant from WaterModel
//     QVector<QPair<QString, double>> topLocations = waterModel->getTopLocationsByPollutant(selectedPollutant);

//     if (topLocations.isEmpty()) {
//         qDebug() << "No data available for the selected pollutant.";
//         return; // Exit if no data is available for the selected pollutant
//     }

//     // Debugging: Check the fetched data
//     qDebug() << "Top Locations for Pollutant:" << selectedPollutant;
//     for (const auto& location : topLocations) {
//         qDebug() << "Location:" << location.first << "Concentration:" << location.second;
//     }

//     // Clear any previous data in the chart
//     chart->removeAllSeries();

//     // Create a new bar series for the chart
//     QBarSeries *series = new QBarSeries();

//     // Prepare data for the chart
//     QBarSet *set = new QBarSet("Concentration");

//     // Add the top 5 locations and their concentrations to the bar set
//     for (const auto& location : topLocations) {
//         set->append(location.second);  // Append the concentration value (second part of QPair)
//     }

//     // Add the bar set to the series
//     series->append(set);

//     // Set the series for the chart
//     chart->addSeries(series);

//     // Set the chart's categories (location names)
//     QStringList categories;
//     for (const auto& location : topLocations) {
//         categories << location.first;  // Add location names to categories
//     }

//     // Set the axis labels for X (Location names)
//     QBarCategoryAxis *axisX = new QBarCategoryAxis();
//     axisX->append(categories);
//     chart->addAxis(axisX, Qt::AlignBottom);  // Updated to addAxis()

//     // Set the axis labels for Y (Concentration values)
//     QValueAxis *axisY = new QValueAxis();
//     axisY->setTitleText("Concentration");
//     axisY->setLabelFormat("%f");  // You can adjust the format for the concentration values
//     chart->addAxis(axisY, Qt::AlignLeft);  // Updated to addAxis()

//     // Attach the axis to the series
//     series->attachAxis(axisX);
//     series->attachAxis(axisY);

//     // Update the chart's title dynamically based on the selected pollutant
//     chart->setTitle("Top 5 Locations for " + selectedPollutant + " Concentration");

//     // Optionally, update other chart settings (colors, animations, etc.)
// }


