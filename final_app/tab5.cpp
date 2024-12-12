#include <QtWidgets>
#include <QHBoxLayout>
#include <QFrame>
#include "styles.hpp"
#include "tab5.hpp"
#include "model.hpp"


Tab5::Tab5(WaterModel* model, QWidget* parent) : QWidget(parent), waterModel(model), complianceLayout(new QHBoxLayout)
{   
    createOverviewLabel();
    createWidgets();
    setupStyles();
    createLayout();

    // Install event filter
    qApp->installEventFilter(this);

    // Connect signal for dropdown selection change
    connect(pollutantDropdown, &QComboBox::currentIndexChanged, this, &Tab5::onPollutantSelectionChanged);
    connect(locationDropdown, &QComboBox::currentIndexChanged, this, &Tab5::onLocationSelectionChanged);
}

//OverviewLabelOverviewLabel
void Tab5::createOverviewLabel()
{
    overviewLabel = new QLabel(tr("Monitor water quality compliance against UK/EU safety standards. "
                                 "Select a pollutant and location to see whether any samples in the dataset exceed regulatory standards i.e <i>the threshold value</i> at that location using color-coded indicators."));
    
    overviewLabel->setWordWrap(true);
    overviewLabel->setAlignment(Qt::AlignCenter);
    overviewLabel->setObjectName("descriptionLabel");

    // Allow the label to resize dynamically
    overviewLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}



void Tab5::onPollutantSelectionChanged(int index)
{
    QString selectedPollutant = pollutantDropdown->itemText(index);
    updateComplianceStatus(selectedPollutant, locationDropdown->currentText());
}

void Tab5::onLocationSelectionChanged(int index)
{
    QString selectedLocation = locationDropdown->itemText(index);
    QString selectedPollutant = pollutantDropdown->currentText(); 

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
    title = new QLabel(tr("Regulatory Compliance Information"));
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
     // Set QSizePolicy to Expanding for dropdowns
    pollutantDropdown->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    locationDropdown->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

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
    complianceCard->setMinimumSize(600, 400); 
    complianceCard->setMaximumSize(800, 800);

    // Layout for the compliance card content
    QVBoxLayout* cardLayout = new QVBoxLayout(complianceCard);
    cardLayout->addWidget(definitionLabel, 0, Qt::AlignLeft);
    cardLayout->addWidget(thresholdLabel, 0, Qt::AlignLeft);
    cardLayout->addWidget(currentValueLabel, 0, Qt::AlignLeft);

    // Legend section
    QLabel* legendLabel = new QLabel(tr("Legend:"));
    QVBoxLayout* legendLayout = new QVBoxLayout;
    QLabel* greenLegend = new QLabel(tr("• Green: Compliant"));
    greenLegend->setStyleSheet("color: green;");
    QLabel* amberLegend = new QLabel(tr("• Amber: Caution"));
    amberLegend->setStyleSheet("color: #FFBF00;");
    QLabel* redLegend = new QLabel(tr("• Red: Non-Compliant"));
    redLegend->setStyleSheet("color: red;");

    legendLayout->addWidget(legendLabel);
    legendLayout->addWidget(greenLegend);
    legendLayout->addWidget(amberLegend);
    legendLayout->addWidget(redLegend);
    cardLayout->addLayout(legendLayout);


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
    
    // Title section with consistent spacing
    mainLayout->addWidget(title);
    mainLayout->addWidget(overviewLabel);
    mainLayout->addSpacing(20);

    // Create horizontal layout for dropdowns
    QHBoxLayout* dropdownLayout = new QHBoxLayout;
    
    // Pollutant section
    QVBoxLayout* pollutantSection = new QVBoxLayout;
    pollutantSection->addWidget(pollutantLabel);
    pollutantSection->addWidget(pollutantDropdown);
    
    // Location section
    QVBoxLayout* locationSection = new QVBoxLayout;
    locationSection->addWidget(locationLabel);
    locationSection->addWidget(locationDropdown);
    
    // Add both sections to horizontal layout
    dropdownLayout->addLayout(pollutantSection);
    dropdownLayout->addLayout(locationSection);
    
    // Add dropdown section to main layout
    mainLayout->addLayout(dropdownLayout);
    mainLayout->addSpacing(20);

    // Add the compliance card
    mainLayout->addWidget(complianceCard, 0, Qt::AlignCenter);

    // Add stretch to fill remaining space
    mainLayout->addStretch();

    // Set margins and spacing
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Set the layout for the widget
    setLayout(mainLayout);
}


void Tab5::setupStyles()
{
    QString styleSheet = QString(R"(
        QLabel#titleLabel {
            font-size: 32px;
            font-weight: bold;
            color: %1;
        }

        QLabel#descriptionLabel {
            font-size: 14px;
            color: %1;
            margin-bottom: 20px;
        }

        QLabel {
            font-size: 18px;
            color: %1;
        }

        QComboBox {
            font-size: 16px;
            padding: 5px;
            background-color: %2;
            color: %1;
            border: 1px solid %3;
        }

        QFrame#complianceCard {
            background-color: white;
            border: 2px solid %3;
            border-radius: 10px;
            padding: 15px;
        }

        QFrame#complianceCard QLabel {
            color: #2c3e50;  /* Always use light mode text color for the card */
        }

        QWidget#trafficLightWidget {
            border-radius: 25px;  
            background-color: %4;
        }
    )")
    .arg(Styles::textColor())                    // %1
    .arg(Styles::backgroundColor())              // %2
    .arg(Styles::borderColor())                  // %3
    .arg(Styles::actionButtonBackground());      // %4

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
    // Define the list of pollutants from the model
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
    "Nitrogen - N",
    "Fluoranthene",
    "Fluoride - F",  // WHO guideline value
    "Fluoroxypyr",
    "HFPO-DA",      // Similar to PFOA/PFOS
    "MeFOSAA-B",
    }; 

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

    // If no pollutants match
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


    // Find the highest concentration for the given pollutant and location
    double maxConcentration = -1.0;
    for (int i = 0; i < waterModel->rowCount(QModelIndex()); ++i) {
        QString samplePollutant = waterModel->data(waterModel->index(i, 4), Qt::DisplayRole).toString();
        QString sampleLocation = waterModel->data(waterModel->index(i, 2), Qt::DisplayRole).toString();

        if (samplePollutant == pollutant && sampleLocation == location) {
            double currentConcentration = waterModel->data(waterModel->index(i, 6), Qt::DisplayRole).toDouble();
            maxConcentration = qMax(maxConcentration, currentConcentration);
        }
    }

    // Update compliance status and traffic light color
    if (maxConcentration < 0.0) {
        complianceStatusLabel->setText(tr("No data available for the selected location and pollutant."));
        currentValueLabel->setText(tr("Current Value: Not available"));
        trafficLightWidget->setStyleSheet("background-color: black;");
        return;
    }

    // Update Current Value label
    currentValueLabel->setText(tr("Current Value: %1").arg(maxConcentration));

    // Determine and display compliance status
    if (maxConcentration <= threshold) {
        complianceStatusLabel->setText(tr("Compliant"));
        trafficLightWidget->setStyleSheet("background-color: rgb(32, 125, 32);");
    } else if (maxConcentration <= threshold * 0.8) {  
        complianceStatusLabel->setText(tr("Near Threshold"));
        trafficLightWidget->setStyleSheet("background-color: rgb(253, 183, 0);");
    } else {
        complianceStatusLabel->setText(tr("Non-Compliant"));
        trafficLightWidget->setStyleSheet("background-color: rgb(204, 31, 40);");
    }

}

