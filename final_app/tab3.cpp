// Implementation for the third card - Environmental Litter Indicator

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


  // area where the graphs are !!!!
  graphContainer = new QWidget;
  graphLayout = new QGridLayout(graphContainer);
  graphLayout->setSpacing(20);
  graphLayout->setContentsMargins(10, 10, 10, 10);
  
  textScrollArea = new QScrollArea(this);

  textScrollArea->setWidgetResizable(true);
  stats = new QTextEdit(textScrollArea); 
  stats->setReadOnly(true);
  stats->setAlignment(Qt::AlignLeft); 
  textScrollArea->setWidget(stats);
  textScrollArea->setWidgetResizable(true);

  scrollArea = new QScrollArea();
  scrollArea->setWidget(graphContainer);
  scrollArea->setWidgetResizable(true);

  createCharts();
}

void Tab3::createCharts()
{
  // Water type Donut chart

  waterTypeSeries_inner = new QPieSeries(); 
  waterTypeSeries_outer = new QPieSeries(); 
  QChart *waterTypeChart = new QChart(); 
  QChart *waterTypeChart2 = new QChart(); 
  waterTypeChart->addSeries(waterTypeSeries_inner);
  waterTypeChart2->addSeries(waterTypeSeries_outer);
  waterTypeChart->setTitle(tr("Water types tested"));
  waterTypeChart2->setTitle(tr("Different types of litter"));
  waterTypeChart->legend()->setAlignment(Qt::AlignRight);
  waterTypeChart2->legend()->setAlignment(Qt::AlignRight);

  for (auto slice : waterTypeSeries_inner->slices()) {
      slice->setLabelVisible(false);  
      slice->setBrush(QColor(50, 150, 255)); 
  }

  for (auto slice : waterTypeSeries_outer->slices()) {
      slice->setLabelVisible(false);  
      slice->setBrush(QColor(200, 100, 255)); 
  }

  waterTypeChartView = new QChartView(waterTypeChart);
  waterTypeChartView2 = new QChartView(waterTypeChart2);
  waterTypeChartView->setRenderHint(QPainter::Antialiasing);
  waterTypeChartView2->setRenderHint(QPainter::Antialiasing);
  waterTypeChartView->setMinimumHeight(300);
  waterTypeChartView2->setMinimumHeight(300);

  graphLayout->addWidget(waterTypeChartView, 0, 0); 
  graphLayout->addWidget(waterTypeChartView2, 0, 1);


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
  litterChartView->setRenderHint(QPainter::Antialiasing);
  litterChartView->setMinimumHeight(300);
  graphLayout->addWidget(litterChartView, 1, 0, 2, 2); 


}


void Tab3::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(placeholder);
  layout->addStretch();
  layout->setSpacing(20);
  layout->setContentsMargins(30, 30, 30, 30);
  layout->addWidget(textScrollArea);
  layout->addWidget(scrollArea);
    
  setLayout(layout);
}

void Tab3::updateCharts()
{

  QMap<QString, QMap<QString, int>*> litterWaterType = waterModel->getLitterWaterType();
  QMap<QString, QMap<QString, int>*> litterLocation = waterModel->getLitterLocation(); // eg [Leeds, [ sewage, 1]]

  // Prepare containers for unique values
  QSet<QString> distinctLocations;
  QSet<QString> distinctLitterTypes;
  QSet<QString> distinctWaterTypes;

  // Extract distinct locations from litterLocation map
  for (auto it = litterLocation.begin(); it != litterLocation.end(); ++it) {
      distinctLocations.insert(it.key());  
  }
  // Extract distinct litter types from litterWaterType map
  for (auto it = litterWaterType.begin(); it != litterWaterType.end(); ++it) {
      for (auto innerIt = it.value()->begin(); innerIt != it.value()->end(); ++innerIt) {
          distinctLitterTypes.insert(innerIt.key());  
      }
  }

  // Extract distinct water types from litterWaterType map
  for (auto it = litterWaterType.begin(); it != litterWaterType.end(); ++it) {
      distinctWaterTypes.insert(it.key());  // Insert water type key
  }
  QRegularExpression regex("\\(\\d+\\)");
  // Format the output for display
  QString locationsText = ("Distinct locations tested for litter: ") + QString::number(distinctLocations.size()) + " - ";
  for (const QString &location : distinctLocations) {
    QString location_mod = location;
      location_mod.remove(regex);
      locationsText += location_mod + ", ";
  }
  locationsText.chop(2);

  QString litterTypesText = ("\nTypes of litter present: ") + QString::number(distinctLitterTypes.size()) + " - ";
  for (const QString &litterType : distinctLitterTypes) {
      litterTypesText += litterType + ", ";
  }
  litterTypesText.chop(2);

  QString waterTypesText = ("\nTypes of bodies of water tested: ") + QString::number(distinctWaterTypes.size()) + " - ";
  for (const QString &waterType : distinctWaterTypes) {
      waterTypesText += waterType + ", ";
  }
  waterTypesText.chop(2);
  // Set the text to display the results
  stats->setText(locationsText + litterTypesText + waterTypesText);
  // // Clear the old data and add the new data DONUT CHART
  waterTypeSeries_inner->clear();
  if (litterWaterType.isEmpty()) {
      qDebug() << "litterWaterType is empty or null!";
      return;  // Early exit to avoid further errors
  }

  int innerWaterTypeTotal = 0;

  for (auto waterTypes : litterWaterType.keys()) {
    int waterCount = 0;
    for (auto litterTypes : litterWaterType.value(waterTypes)->keys()) {
      waterCount += litterWaterType.value(waterTypes)->value(litterTypes);
    }
    innerWaterTypeTotal += waterCount;
    waterTypeSeries_inner->append(waterTypes, waterCount);
    qDebug() << "Water Type: " << waterTypes;
  }


  waterTypeSeries_outer->clear();
  QMap<QString, int> litterData_forWaterType; // eg {"Leeds",  3}

  for (auto waterType : litterWaterType.keys()) {
    //loop through trash type for each water type
    for (auto litterType : litterWaterType.value(waterType)->keys()) {
      // get amount of current type of trash for this water type
      int litterCount = litterWaterType.value(waterType)->value(litterType);
      //add count of litter to water type
      if (litterData_forWaterType.contains(litterType)) {
        litterData_forWaterType[litterType] += litterCount;
      } else {
        litterData_forWaterType.insert(litterType, litterCount);
      }
    }
  }
  for (auto litterType : litterData_forWaterType.keys()) {
    int totalCount = litterData_forWaterType.value(litterType);
    waterTypeSeries_outer->append(litterType, totalCount);
  }


// BAR CHART --------------
  litterSeries->clear();


  // retrieves data about litter location from watermodel using func, then stores it in *litterLocation * variable
  //QMap<QString, QMap<QString, int>*> litterLocation = waterModel->getLitterLocation(); // eg [Leeds, [ sewage, 1]]
    
  // Create a single bar set for all litters
  QBarSet *set = new QBarSet(tr("Amount of Litter"));
  
  // empty list that holds the labels for x-axis of the chart
  QStringList categories; 
  

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
