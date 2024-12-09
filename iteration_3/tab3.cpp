// Implementation for the third card - Environmental Litter Indicator
// Mila
/*
ENVIRONMENTAL LITTER INDICATORS:
PURPOSE: Summarises physical pollutants such as plastic litter and other visible debris in water
- Litter comparisons - provide comparisons of litter levels across different litters and water body types, using bar or pie charts.
- Compliance Indicators - Show adherence to EU Bathing Water standards regarding litter presence
- Interaction - Allow people to filter by litter or type of litter to explore trends.
NEED:
- determindand.label: TarryResidus(9691)[pres/nf], BWP - O.L.(4826)[gerber c], SewageDebris(9156)[gerber c] 
      {bracket is determindand.notation, square is determinand.unit.label}
- determindand.notation:  (9691), (4826), (9156)
- determinand.unit.label:   [pres/nf], [gerber c], [gerber c] 
- determindand.definition:  Bathing Water Profile : Other Litter (incl. plastics), Sewage debris, Tarry residues
- sample.samplingPoint.label:   litters
- sample.sampledMaterialType.label:   water type: RIver Running and sea water
- sample.isComplianceSample: False/True
- sample.purpose.label:   why


To create a chart:

    Modify model.hpp and model.cpp as follows:
        In model.hpp add the variable you want to use to store the chart data under private and create a public getter for it
        In model.cpp, construct the variable in the method `dataProcessing()` in the main loop
    Modify tab1.hpp and tab1.cpp and window.cpp follows:
        Follow the example in tab1.hpp and tab1.cpp to create new charts for your tab, note the modifications to the `createWidgets()`, `createLayout()`, `createCharts()` and `updateCharts()` methods
        `createCharts()` just creates dummy charts with no data, these will be updated on each data load
        In window.cpp add `yourTab->updateCharts();` below `tab1->updateCharts()` in the `loadData()` method

Add charts one at a time and always recompile the program to see the changes. As always, wrap all user facing strings in `tr()` to allow for easy translation in the future. 
*/

//helpful
// filterAcceptsRow()
// what litters have good, bad  and filter between different litter type
// litterLocation eg: { "Leeds", { {"sewage", 1}, {"plastic", 3} } }
// 1217 unique ids
// two water types
// 

#include <QtWidgets>
#include "styles.hpp"
#include "tab3.hpp"


Tab3::Tab3(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
  setupStyles();
  
  // Install event filter for theme changes
  qApp->installEventFilter(this);
}

bool Tab3::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
    setupStyles();
    return true;
  }
  return QWidget::eventFilter(watched, event);
}

void Tab3::createWidgets()
{
  placeholder = new QLabel(tr("Environmental Litter Indicators - Coming Soon (Mila)"));
  QFont labelFont;
  labelFont.setPointSize(32);
  labelFont.setBold(true);
  placeholder->setFont(labelFont);
  placeholder->setAlignment(Qt::AlignCenter);
  placeholder->setObjectName("placeholderLabel");

  // filter button

  // area where the graphs are
  // graphContainer = new QWidget;
  // graphLayout = new QGridLayout(graphContainer);
  // graphLayout->setSpacing(20);
  // graphLayout->setContentsMargins(20, 20, 20, 20);

  // scrollArea = new QScrollArea;
  // scrollArea->setWidget(graphContainer);
  // scrollArea->setWidgetResizable(true);
  createCharts();
}

void Tab3::createCharts() //STOLEN
{
  // Water type Donut chart

    //    QChartView *waterTypeChartView; // was complianceChartView
    //    QPieSeries *waterTypeSeries; // was complianceSeries
    //    
  waterTypeSeries = new QPieSeries(); 
  QChart *waterTypeChart = new QChart(); // was complianceChart
  waterTypeChart->addSeries(waterTypeSeries);
  waterTypeChart->setTitle(tr("Water types and their litters"));
  waterTypeChart->legend()->setAlignment(Qt::AlignRight);
  waterTypeChartView = new QChartView(waterTypeChart);
  waterTypeChartView->setRenderHint(QPainter::Antialiasing);
  waterTypeChartView->setMinimumHeight(300);
  //chartLayout->addWidget(waterTypeChartView, 0, 0);

  // LITTER BAR CHART_______________
  //new instance of QBarSeries(from hpp). *litterSeries* now points to the new QBarSeries object which holds bar data to be plotted on the chart
  litterSeries = new QBarSeries();
  // creates new QChart object which manages the chart. *litterChart* is a pointer to the QChart object
  QChart *litterChart = new QChart();
  litterChart->addSeries(litterSeries);
  litterChart->setTitle(tr("Litter per Location"));
  litterChart->setAnimationOptions(QChart::SeriesAnimations);

  QStringList categories;
  categories << "Loading...";
  QBarSet *set = new QBarSet("");
  *set << 0;
  litterSeries->append(set);

  QBarCategoryAxis *axisX = new QBarCategoryAxis();
  axisX->append(categories);
  axisX->setLabelsAngle(270); // makes the text verticle
  litterChart->addAxis(axisX, Qt::AlignBottom);
  litterSeries->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();
  axisY->setRange(0, 100);
  litterChart->addAxis(axisY, Qt::AlignLeft);
  litterSeries->attachAxis(axisY);

  litterChart->legend()->setVisible(false);
  litterChartView = new QChartView(litterChart);
  //litterChartView->setRenderHint(QPainter::Antialiasing); remove
  litterChartView->setMinimumHeight(300);
  //chartLayout->addWidget(litterChartView, 0, 1); remove


  // // Determinands Bar Chart
  // determinandSeries = new QBarSeries();
  // QChart *determinandChart = new QChart();
  // determinandChart->addSeries(determinandSeries);
  // determinandChart->setTitle(tr("Top Determinands Sampled"));
  // determinandChart->setAnimationOptions(QChart::SeriesAnimations);

  // QStringList categories2;
  // categories2 << "Loading...";
  // QBarSet *set2 = new QBarSet("");
  // *set2 << 0;
  // determinandSeries->append(set2);

  // QBarCategoryAxis *axisX2 = new QBarCategoryAxis();
  // axisX2->append(categories2);
  // determinandChart->addAxis(axisX2, Qt::AlignBottom);
  // determinandSeries->attachAxis(axisX2);

  // QValueAxis *axisY2 = new QValueAxis();
  // axisY2->setRange(0, 100);
  // determinandChart->addAxis(axisY2, Qt::AlignLeft);
  // determinandSeries->attachAxis(axisY2);

  // determinandChart->legend()->setVisible(false);
  // determinandChartView = new QChartView(determinandChart);
  // determinandChartView->setRenderHint(QPainter::Antialiasing);
  // determinandChartView->setMinimumHeight(300);
  // chartLayout->addWidget(determinandChartView, 1, 0);

  // // Timeline Line Chart
  // timelineSeries = new QLineSeries();
  // QChart *timelineChart = new QChart();
  // timelineChart->addSeries(timelineSeries);
  // timelineChart->setTitle(tr("Samples Collected Per Date"));

  // // Create axes with improved settings
  // QDateTimeAxis *axisX3 = new QDateTimeAxis;
  // axisX3->setFormat("yyyy-MM-dd");
  // axisX3->setTitleText(tr("Date"));
  // timelineChart->addAxis(axisX3, Qt::AlignBottom);
  // timelineSeries->attachAxis(axisX3);

  // QValueAxis *axisY3 = new QValueAxis;
  // axisY3->setLabelFormat("%d");
  // axisY3->setTitleText(tr("Number of Samples"));
  // axisY3->setMinorTickCount(0);  // Disable minor ticks
  // axisY3->setLabelsVisible(true);
  // axisY3->setMin(0);  // Always start from 0
  // timelineChart->addAxis(axisY3, Qt::AlignLeft);
  // timelineSeries->attachAxis(axisY3);

  // // Add some padding to prevent label clipping
  // timelineChart->setMargins(QMargins(10, 10, 10, 10));
  // timelineChart->layout()->setContentsMargins(0, 0, 0, 0);

  // timelineChart->legend()->setVisible(false);
  // timelineChartView = new QChartView(timelineChart);
  // timelineChartView->setRenderHint(QPainter::Antialiasing);
  // timelineChartView->setMinimumHeight(300);
  // chartLayout->addWidget(timelineChartView, 1, 1);
}


void Tab3::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(placeholder);
  layout->addStretch();
  layout->setSpacing(20);
  layout->setContentsMargins(30, 30, 30, 30);
  //layout->addWidget(stats, 0, Qt::AlignCenter);
  
  layout->addWidget(litterChartView);
  layout->addWidget(waterTypeChartView);
    
  setLayout(layout);
}

// These should probably be their own functions tbh
// i.e. updateComplianceChart(), updatelitterChart(), updateDeterminandChart()
void Tab3::updateCharts()
{

  // // Clear the old data and add the new data DONUT CHART
  waterTypeSeries->clear();
  QMap<QString, QMap<QString, int>*> litterWaterType = waterModel->getLitterWaterType();
  int outerWaterTypeTotal = 0;

  for (auto waterTypes : litterWaterType.keys()) {
    int waterCount = 0;
    for (auto litterType : litterWaterType.value(waterTypes)->keys()) {
      waterCount += litterWaterType.value(waterTypes)->value(litterType);
    }
    outerWaterTypeTotal += waterCount;
    waterTypeSeries->append(waterTypes, waterCount);
  }
  // waterTypeSeries->append(tr("Compliance"), waterModel->getComplianceSampleCount().first);
  // waterTypeSeries->append(tr("Other Samples"), waterModel->getComplianceSampleCount().second);
  // waterTypeSeries->setHoleSize(0.35);


// BAR CHART --------------
  litterSeries->clear();


  // retrieves data about litter location from watermodel using func, then stores it in *litterLocation * variable
  QMap<QString, QMap<QString, int>*> litterLocation = waterModel->getLitterLocation(); // eg [Leeds, [ sewage, 1]]
    
  // Create a single bar set for all litters
  QBarSet *set = new QBarSet("Amount of Litter");
  
  // empty list that holds the labels for x-axis of the chart
  QStringList categories; 
  
  // from web
  QMap<QString, int> litterData; // eg {"Leeds",  3}

  // Iterate through the initial map and calculate the sum of inner QMaps
    // it.key() is a location like "Leeds"
    // it.value() is a pointer to the inner QMap which holds type of litter and count {"sewage", 3}
  for (auto it = litterLocation.cbegin(); it != litterLocation.cend(); ++it) {
    int sum = 0;
      for (const auto& value : *(it.value())) {
        sum += value;
        }
    litterData[it.key()] = sum;
      // e.g: litterLocation["Leeds"] = { {"sewage", 1}, {"plastic", 3} }, 
            //litterData["Leeds"] = 4.
  }
 // end from web
    
  // Add data to the set and categories
  for (auto it = litterData.constBegin(); it != litterData.constEnd(); ++it) {
    categories << it.key();
    *set << it.value();
  }
    
  // Add the set to the series
  litterSeries->append(set);
    
  // Update the axis categories
  QBarCategoryAxis *axis = qobject_cast<QBarCategoryAxis*>(
    litterChartView->chart()->axes(Qt::Horizontal).first()
  );
  axis->clear();
  axis->append(categories);
    
  // Update y-axis range
  QValueAxis *valueAxis = qobject_cast<QValueAxis*>(
    litterChartView->chart()->axes(Qt::Vertical).first()
  );
  int maxValue = *std::max_element(litterData.begin(), litterData.end());
  valueAxis->setRange(0, maxValue * 1.1);
  valueAxis->setLabelFormat("%d");

}


void Tab3::setupStyles()
{
  QString styleSheet = QString(R"(
    QWidget {
      background-color: transparent;
      color: %1;
    }

    QLabel#placeholderLabel {
      font-size: 32px;
      font-weight: bold;
      color: %1;
    }
  )")
  .arg(Styles::textColor());        // %1

  this->setStyleSheet(styleSheet);
}
