// Implementation for the first card - Dashboard
// Jamil

#include <QtWidgets>
#include "styles.hpp"
#include "tab1.hpp"


Tab1::Tab1(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
  setupStyles();
  
  // Install event filter for Styles changes
  qApp->installEventFilter(this);
}

bool Tab1::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
    setupStyles();
    return true;
  }
  return QWidget::eventFilter(watched, event);
}

void Tab1::createWidgets()
{
  reloadButton = new QPushButton(tr("Reload Data"));
  reloadButton->setMinimumWidth(200);
  reloadButton->setMinimumHeight(45);
  reloadButton->setCursor(Qt::PointingHandCursor);
  reloadButton->setObjectName("actionButton");
  connect(reloadButton, SIGNAL(clicked()), this, SIGNAL(reloadRequested()));

  title = new QLabel(tr("Dashboard"));
  QFont labelFont;
  labelFont.setPointSize(32);
  labelFont.setBold(true);
  title->setFont(labelFont);
  title->setAlignment(Qt::AlignCenter);
  title->setObjectName("titleLabel");

  stats = new QLabel();
  stats->setAlignment(Qt::AlignCenter);

  // Chart creation and layout
  chartContainer = new QWidget;
  chartLayout = new QGridLayout(chartContainer);
  chartLayout->setSpacing(20);
  chartLayout->setContentsMargins(20, 20, 20, 20);

  scrollArea = new QScrollArea;
  scrollArea->setWidget(chartContainer);
  scrollArea->setWidgetResizable(true);

  createCharts();
}

// These should maybe be their own functions?
void Tab1::createCharts()
{
  // Compliance Donut Chart
  complianceSeries = new QPieSeries();
  QChart *complianceChart = new QChart();
  complianceChart->addSeries(complianceSeries);
  complianceChart->setTitle(tr("Compliance vs. Other Samples"));
  complianceChart->legend()->setAlignment(Qt::AlignRight);
  complianceChartView = new QChartView(complianceChart);
  complianceChartView->setRenderHint(QPainter::Antialiasing);
  complianceChartView->setMinimumHeight(300);
  chartLayout->addWidget(complianceChartView, 0, 0);

  // Location Bar Chart
  locationSeries = new QBarSeries();
  QChart *locationChart = new QChart();
  locationChart->addSeries(locationSeries);
  locationChart->setTitle(tr("Top Sampling Locations"));
  locationChart->setAnimationOptions(QChart::SeriesAnimations);

  QStringList categories;
  categories << "Loading...";
  QBarSet *set = new QBarSet("");
  *set << 0;
  locationSeries->append(set);

  QBarCategoryAxis *axisX = new QBarCategoryAxis();
  axisX->append(categories);
  locationChart->addAxis(axisX, Qt::AlignBottom);
  locationSeries->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();
  axisY->setRange(0, 100);
  locationChart->addAxis(axisY, Qt::AlignLeft);
  locationSeries->attachAxis(axisY);

  locationChart->legend()->setVisible(false);
  locationChartView = new QChartView(locationChart);
  locationChartView->setRenderHint(QPainter::Antialiasing);
  locationChartView->setMinimumHeight(300);
  chartLayout->addWidget(locationChartView, 0, 1);

  // Determinands Bar Chart
  determinandSeries = new QBarSeries();
  QChart *determinandChart = new QChart();
  determinandChart->addSeries(determinandSeries);
  determinandChart->setTitle(tr("Top Determinands Sampled"));
  determinandChart->setAnimationOptions(QChart::SeriesAnimations);

  QStringList categories2;
  categories2 << "Loading...";
  QBarSet *set2 = new QBarSet("");
  *set2 << 0;
  determinandSeries->append(set2);

  QBarCategoryAxis *axisX2 = new QBarCategoryAxis();
  axisX2->append(categories2);
  determinandChart->addAxis(axisX2, Qt::AlignBottom);
  determinandSeries->attachAxis(axisX2);

  QValueAxis *axisY2 = new QValueAxis();
  axisY2->setRange(0, 100);
  determinandChart->addAxis(axisY2, Qt::AlignLeft);
  determinandSeries->attachAxis(axisY2);

  determinandChart->legend()->setVisible(false);
  determinandChartView = new QChartView(determinandChart);
  determinandChartView->setRenderHint(QPainter::Antialiasing);
  determinandChartView->setMinimumHeight(300);
  chartLayout->addWidget(determinandChartView, 1, 0);

  // Timeline Line Chart
  timelineSeries = new QLineSeries();
  QChart *timelineChart = new QChart();
  timelineChart->addSeries(timelineSeries);
  timelineChart->setTitle(tr("Samples Collected Per Date"));

  // Create axes with improved settings
  QDateTimeAxis *axisX3 = new QDateTimeAxis;
  axisX3->setFormat("yyyy-MM-dd");
  axisX3->setTitleText(tr("Date"));
  timelineChart->addAxis(axisX3, Qt::AlignBottom);
  timelineSeries->attachAxis(axisX3);

  QValueAxis *axisY3 = new QValueAxis;
  axisY3->setLabelFormat("%d");
  axisY3->setTitleText(tr("Number of Samples"));
  axisY3->setMinorTickCount(0);  // Disable minor ticks
  axisY3->setLabelsVisible(true);
  axisY3->setMin(0);  // Always start from 0
  timelineChart->addAxis(axisY3, Qt::AlignLeft);
  timelineSeries->attachAxis(axisY3);

  // Add some padding to prevent label clipping
  timelineChart->setMargins(QMargins(10, 10, 10, 10));
  timelineChart->layout()->setContentsMargins(0, 0, 0, 0);

  timelineChart->legend()->setVisible(false);
  timelineChartView = new QChartView(timelineChart);
  timelineChartView->setRenderHint(QPainter::Antialiasing);
  timelineChartView->setMinimumHeight(300);
  chartLayout->addWidget(timelineChartView, 1, 1);
}

void Tab1::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(title);
  layout->addWidget(reloadButton, 0, Qt::AlignCenter);
  layout->addWidget(stats, 0, Qt::AlignCenter);
  layout->addWidget(scrollArea);
  layout->setSpacing(20);
  layout->setContentsMargins(30, 30, 30, 30);
    
  setLayout(layout);
}

// These should probably be their own functions tbh
// i.e. updateComplianceChart(), updateLocationChart(), updateDeterminandChart()
void Tab1::updateCharts()
{
  stats->setText(
    tr("Distinct Samples Taken: ") + QString("%L1").arg(waterModel->rowCount(QModelIndex())) + "\n" +
    tr("Distinct Locations Measured: ") + QString("%L1").arg(waterModel->getLocationCount()) + "\n" +
    tr("Distinct Determinands Sampled: ") + QString("%L1").arg(waterModel->getDeterminandCount()) + "\n\n" +
    tr("Last Updated: ") + QDateTime::currentDateTime().toString("yyyy-MM-dd, hh:mm:ss")
  );

  // Clear the old data and add the new data
  complianceSeries->clear();
  complianceSeries->append(tr("Compliance"), waterModel->getComplianceSampleCount().first);
  complianceSeries->append(tr("Other Samples"), waterModel->getComplianceSampleCount().second);
  complianceSeries->setHoleSize(0.35);

  locationSeries->clear();
  QMap<QString, int> locationData = waterModel->getSampleLocations();
    
  // Create a single bar set for all locations
  QBarSet *set = new QBarSet("Samples");
  QStringList categories;
    
  // Add data to the set and categories
  for (auto it = locationData.constBegin(); it != locationData.constEnd(); ++it) {
    categories << it.key();
    *set << it.value();
  }
    
  // Add the set to the series
  locationSeries->append(set);
    
  // Update the axis categories
  QBarCategoryAxis *axis = qobject_cast<QBarCategoryAxis*>(
    locationChartView->chart()->axes(Qt::Horizontal).first()
  );
  axis->clear();
  axis->append(categories);
    
  // Update y-axis range
  QValueAxis *valueAxis = qobject_cast<QValueAxis*>(
    locationChartView->chart()->axes(Qt::Vertical).first()
  );
  int maxValue = *std::max_element(locationData.begin(), locationData.end());
  valueAxis->setRange(0, maxValue * 1.1);
  valueAxis->setLabelFormat("%d");

  determinandSeries->clear();
  QMap<QString, int> determinandData = waterModel->getDeterminandCounts();

  // Create a single bar set for all determinands
  QBarSet *set2 = new QBarSet("Samples");
  QStringList categories2;

  // Add data to the set and categories
  for (auto it = determinandData.constBegin(); it != determinandData.constEnd(); ++it) {
    categories2 << it.key();
    *set2 << it.value();
  }

  // Add the set to the series
  determinandSeries->append(set2);

  // Update the axis categories
  QBarCategoryAxis *axis2 = qobject_cast<QBarCategoryAxis*>(
    determinandChartView->chart()->axes(Qt::Horizontal).first()
  );
  axis2->clear();
  axis2->append(categories2);

  // Update y-axis range
  QValueAxis *valueAxis2 = qobject_cast<QValueAxis*>(
    determinandChartView->chart()->axes(Qt::Vertical).first()
  );
  int maxValue2 = *std::max_element(determinandData.begin(), determinandData.end());
  valueAxis2->setRange(0, maxValue2 * 1.1);
  valueAxis2->setLabelFormat("%d"); 

  // Update timeline chart
  timelineSeries->clear();
  QMap<QDate, int> dateData = waterModel->getSampleDates();

  for(auto it = dateData.constBegin(); it != dateData.constEnd(); ++it) {
    qint64 msecsSinceEpoch = it.key().startOfDay().toMSecsSinceEpoch();
    timelineSeries->append(msecsSinceEpoch, it.value());
  }

  // Update axes ranges with improved visibility
  if(!dateData.isEmpty()) {
    QDateTimeAxis *axisX = qobject_cast<QDateTimeAxis*>(
      timelineChartView->chart()->axes(Qt::Horizontal).first()
    );
    axisX->setRange(dateData.firstKey().startOfDay(), dateData.lastKey().endOfDay());

    QValueAxis *axisY = qobject_cast<QValueAxis*>(
      timelineChartView->chart()->axes(Qt::Vertical).first()
    );
    int maxValue = *std::max_element(dateData.begin(), dateData.end());
    
    // Add 20% padding to the top to ensure labels don't get cut off
    axisY->setRange(0, maxValue * 1.2);
    
    // Adjust the number of tick marks based on the maximum value
    int tickCount = std::min(10, maxValue + 1);  // Cap at 10 tick marks
    axisY->setTickCount(tickCount);
  }
}

void Tab1::setupStyles()
{
  QString styleSheet = QString(R"(
    QWidget {
      background-color: transparent;
      color: %1;
    }

    QPushButton#actionButton {
      background-color: %2;
      border: none;
      border-radius: %3px;
      color: white;
      font-weight: bold;
      padding: %4px %5px;
      min-width: %6px;
      min-height: %7px;
    }

    QPushButton#actionButton:hover {
      background-color: %8;
    }

    QPushButton#actionButton:disabled {
      background-color: %9;
      color: white;
    }

    QLabel#titleLabel {
      font-size: 32px;
      font-weight: bold;
      color: %1;
    }
  )")
  .arg(Styles::textColor())                    // %1
  .arg(Styles::actionButtonBackground())       // %2
  .arg(Styles::BORDER_RADIUS)                  // %3
  .arg(Styles::PADDING_MEDIUM)                 // %4
  .arg(Styles::PADDING_LARGE)                  // %5
  .arg(Styles::ACTION_BUTTON_WIDTH)            // %6
  .arg(Styles::ACTION_BUTTON_HEIGHT)           // %7
  .arg(Styles::actionButtonHover())            // %8
  .arg(Styles::actionButtonDisabled());        // %9

  this->setStyleSheet(styleSheet);
}
