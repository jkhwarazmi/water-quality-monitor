// Implementation for the third card - Environmental Litter Indicator
// Mila

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
  title = new QLabel(tr("Environmental Litter Indicators"));
  QFont labelFont;
  labelFont.setPointSize(32);
  labelFont.setBold(true);
  title->setFont(labelFont);
  title->setAlignment(Qt::AlignCenter);
  title->setObjectName("placeholderLabel");

  description = new QLabel(tr("Monitor number of litter measurements in water samples per location and water type."));
  description->setWordWrap(true);
  description->setAlignment(Qt::AlignCenter);
  description->setObjectName("descriptionLabel");
  QFont descriptionFont;
  descriptionFont.setPointSize(14); 
  description->setFont(descriptionFont);

  QFont errorFont;
  errorFont.setPointSize(14);

  // Create error labels, hidden by default
  litterChartError = new QLabel(tr("No litter location data available"));
  litterChartError->setAlignment(Qt::AlignCenter);
  litterChartError->hide();

  waterTypeChartError = new QLabel(tr("No water type data available"));
  waterTypeChartError->setAlignment(Qt::AlignCenter);
  waterTypeChartError->hide(); 

  createCharts();
}

void Tab3::createCharts()
{  
  waterTypeSeries = new QPieSeries(); 
  QChart *waterTypeChart = new QChart(); // was complianceChart
  waterTypeChart->addSeries(waterTypeSeries);
  waterTypeChart->setTitle(tr("Water types and their litters"));
  waterTypeChart->legend()->setAlignment(Qt::AlignRight);
  waterTypeChartView = new QChartView(waterTypeChart);
  waterTypeChartView->setRenderHint(QPainter::Antialiasing);
  waterTypeChartView->setMinimumHeight(300);

  litterSeries = new QBarSeries();
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
  axisX->setLabelsAngle(270);
  litterChart->addAxis(axisX, Qt::AlignBottom);
  litterSeries->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();
  axisY->setRange(0, 100);
  litterChart->addAxis(axisY, Qt::AlignLeft);
  litterSeries->attachAxis(axisY);

  litterChart->legend()->setVisible(false);
  litterChartView = new QChartView(litterChart);
  litterChartView->setMinimumHeight(300);
}


void Tab3::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(title);
  layout->addWidget(description);
  layout->setSpacing(20);
  layout->setContentsMargins(30, 30, 30, 30);
  
  layout->addWidget(litterChartView);
  layout->addWidget(litterChartError);
  layout->addWidget(waterTypeChartView);
  layout->addWidget(waterTypeChartError);
    
  setLayout(layout);
}

void Tab3::updateCharts()
{
  // Water type pie chart
  waterTypeSeries->clear();
  QMap<QString, QMap<QString, int>*> litterWaterType = waterModel->getLitterWaterType();
  int totalSamples = 0;

  // First calculate total for percentage calculation
  for (auto waterTypes : litterWaterType.keys()) {
    int waterCount = 0;
    for (auto litterType : litterWaterType.value(waterTypes)->keys()) {
      waterCount += litterWaterType.value(waterTypes)->value(litterType);
    }
    totalSamples += waterCount;
  }

  // Show/hide chart based on data availability
  if (totalSamples > 0) {
    waterTypeChartView->show();
    waterTypeChartError->hide();
    
    for (auto waterTypes : litterWaterType.keys()) {
      int waterCount = 0;
      for (auto litterType : litterWaterType.value(waterTypes)->keys()) {
        waterCount += litterWaterType.value(waterTypes)->value(litterType);
      }
      double percentage = (waterCount * 100.0) / totalSamples;
      waterTypeSeries->append(
        tr("%1 (%2%)").arg(waterTypes).arg(QString::number(percentage, 'f', 1)),
        waterCount
      );
    }
    waterTypeSeries->setHoleSize(0.35);
  } else {
    waterTypeChartView->hide();
    waterTypeChartError->show();
  }

  // BAR CHART --------------
  litterSeries->clear();
  QMap<QString, QMap<QString, int>*> litterLocation = waterModel->getLitterLocation();
  QMap<QString, int> litterData;

  // Calculate totals per location
  for (auto it = litterLocation.cbegin(); it != litterLocation.cend(); ++it) {
    int sum = 0;
    for (const auto& value : *(it.value())) {
      sum += value;
    }
    litterData[it.key()] = sum;
  }

  // Show/hide chart based on data availability
  if (litterData.isEmpty()) {
    litterChartView->hide();
    litterChartError->show();
    return;  // Exit early if no data
  }

  litterChartView->show();
  litterChartError->hide();

  QBarSet *set = new QBarSet("Amount of Litter");
  QStringList categories;

  // Add data to the set and categories
  for (auto it = litterData.constBegin(); it != litterData.constEnd(); ++it) {
    categories << it.key();
    *set << it.value();
  }

  litterSeries->append(set);

  // Add tooltip handling
  connect(litterSeries, &QBarSeries::hovered,
    [this, categories](bool status, int index, QBarSet *barset) {
      if (status && index >= 0 && index < categories.size()) {
        QString tooltipText = QString("%1\nAmount: %2")
          .arg(categories[index])
          .arg(barset->at(index));
        
        QToolTip::showText(
          QCursor::pos(),
          tooltipText,
          litterChartView
        );
      } else {
        QToolTip::hideText();
      }
  });

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
