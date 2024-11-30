// Implementation for the first card - Dashboard
// Jamil

#include <QtWidgets>
#include "styles.hpp"
#include "tab1.hpp"


Tab1::Tab1(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  // Initialize basic UI elements
  createWidgets();
  
  // Defer chart creation to avoid UI freeze
  QTimer::singleShot(0, this, &Tab1::initCharts);
  
  // Install event filter for Styles changes
  qApp->installEventFilter(this);
}

void Tab1::initCharts()
{
  // Create charts
  complianceChart = createComplianceStatusChart();
  purposeChart = createPurposeChart();
  pollutantsChart = createPollutantsChart();
  materialsChart = createMaterialsChart();
  categoriesChart = createCategoriesChart();
  
  // Set up layout after charts are created
  createLayout();
  setupStyles();
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

  headingLabel = new QLabel(tr("Dashboard"));
  QFont labelFont;
  labelFont.setPointSize(32);
  labelFont.setBold(true);
  headingLabel->setFont(labelFont);
  headingLabel->setAlignment(Qt::AlignCenter);
  headingLabel->setObjectName("headingLabel");
}

void Tab1::createLayout()
{
  // Main layout
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setSpacing(20);
  layout->setContentsMargins(30, 30, 30, 30);
  
  // Add title and reload button at the top
  layout->addWidget(headingLabel);
  layout->addWidget(reloadButton, 0, Qt::AlignRight);

  // Create scroll area for charts
  QScrollArea *scrollArea = new QScrollArea;
  scrollArea->setWidgetResizable(true);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  
  // Create widget to hold charts
  QWidget *scrollContent = new QWidget;
  QGridLayout* chartsLayout = new QGridLayout(scrollContent);
  chartsLayout->setSpacing(20);

  // Configure chart sizes
  const int chartMinHeight = 300;  // Minimum height for charts
  
  // Set size policies and minimum sizes for charts
  QList<QChartView*> charts = {complianceChart, purposeChart, materialsChart, 
                              categoriesChart, pollutantsChart};
  for (QChartView* chart : charts) {
    if (chart) {
      chart->setMinimumHeight(chartMinHeight);
      chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      chart->chart()->setMinimumSize(10, 10);  // Prevent chart from disappearing
    }
  }

  // Add charts to layout with proper sizing
  chartsLayout->addWidget(complianceChart, 0, 0);
  chartsLayout->addWidget(purposeChart, 0, 1);
  chartsLayout->addWidget(materialsChart, 1, 0);
  chartsLayout->addWidget(categoriesChart, 1, 1);
  chartsLayout->addWidget(pollutantsChart, 2, 0, 1, 2);

  // Set column stretches to make charts fill space evenly
  chartsLayout->setColumnStretch(0, 1);
  chartsLayout->setColumnStretch(1, 1);

  scrollContent->setLayout(chartsLayout);
  scrollArea->setWidget(scrollContent);
  layout->addWidget(scrollArea);

  setLayout(layout);
}

QChartView* Tab1::createComplianceStatusChart() 
{
  const QMap<QString, int>& complianceCounts = waterModel->getComplianceCounts();
  
  // Create and configure series
  QPieSeries* series = new QPieSeries();
  
  // Add data with proper labels
  qreal complianceValue = complianceCounts.value("Yes", 0);
  qreal otherValue = complianceCounts.value("No", 0);
  
  if (complianceValue > 0 || otherValue > 0) {  // Only add if we have data
    QPieSlice *slice1 = new QPieSlice(tr("Compliance"), complianceValue);
    QPieSlice *slice2 = new QPieSlice(tr("Other"), otherValue);
    
    slice1->setLabelVisible(true);
    slice2->setLabelVisible(true);
    
    // Add percentage labels
    qreal total = complianceValue + otherValue;
    if (total > 0) {
      slice1->setLabel(QString("%1: %2%")
        .arg(tr("Compliance"))
        .arg(QString::number((complianceValue/total) * 100, 'f', 1)));
      slice2->setLabel(QString("%1: %2%")
        .arg(tr("Other"))
        .arg(QString::number((otherValue/total) * 100, 'f', 1)));
    }
    
    series->append(slice1);
    series->append(slice2);
  }
  
  // Create and configure chart
  QChart* chart = new QChart();
  chart->addSeries(series);
  chart->setTitle(tr("Proportion of Compliance Measurements"));
  chart->legend()->setVisible(true);
  chart->legend()->setAlignment(Qt::AlignBottom);
  
  // Remove chart margins to maximize space usage
  chart->layout()->setContentsMargins(0, 0, 0, 0);
  chart->setMargins(QMargins(0, 0, 0, 0));
  
  // Create chart view with proper sizing
  QChartView* chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  return chartView;
}

QChartView* Tab1::createPurposeChart() 
{
  const QMap<QString, int>& purposeCounts = waterModel->getPurposeCounts();
  
  QBarSeries* series = new QBarSeries();
  QBarSet* barSet = new QBarSet(tr("Count"));
  QStringList categories;
  
  categories.reserve(purposeCounts.size());
  
  for (auto it = purposeCounts.constBegin(); it != purposeCounts.constEnd(); ++it) {
    barSet->append(it.value());
    categories << it.key();
  }
  
  series->append(barSet);
  series->setLabelsVisible(true);
  
  QChart* chart = new QChart();
  chart->addSeries(series);
  chart->setTitle(tr("Sample Purposes"));
  
  QBarCategoryAxis* axisX = new QBarCategoryAxis();
  axisX->append(categories);
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);
  
  QValueAxis* axisY = new QValueAxis();
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);
  
  chart->legend()->setVisible(true);
  
  QChartView* chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  return chartView;
}

QChartView* Tab1::createPollutantsChart() 
{
  const QList<QPair<QString, double>> topPollutants = waterModel->getTopPollutants();
  const QMap<QString, QString>& pollutantUnits = waterModel->getPollutantUnits();
  
  QHorizontalBarSeries* series = new QHorizontalBarSeries();
  QBarSet* barSet = new QBarSet(tr("Average Value"));
  QStringList categories;
  
  int count = qMin(5, topPollutants.size());
  categories.reserve(count);
  
  for (int i = 0; i < count; ++i) {
    barSet->append(topPollutants[i].second);
    categories << topPollutants[i].first + " (" + pollutantUnits[topPollutants[i].first] + ")";
  }
  
  series->append(barSet);
  series->setLabelsVisible(true);
  
  QChart* chart = new QChart();
  chart->addSeries(series);
  chart->setTitle(tr("Top 5 Pollutants by Average Concentration"));
  
  QBarCategoryAxis* axisY = new QBarCategoryAxis();
  axisY->append(categories);
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);
  
  QValueAxis* axisX = new QValueAxis();
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);
  
  chart->legend()->setVisible(true);
  
  QChartView* chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  return chartView;
}

QChartView* Tab1::createMaterialsChart() 
{
  const QMap<QString, int>& materialCounts = waterModel->getMaterialTypeCounts();
  
  QBarSeries* series = new QBarSeries();
  QBarSet* barSet = new QBarSet(tr("Count"));
  QStringList categories;
  
  categories.reserve(materialCounts.size());
  
  for (auto it = materialCounts.constBegin(); it != materialCounts.constEnd(); ++it) {
    barSet->append(it.value());
    categories << it.key();
  }
  
  series->append(barSet);
  series->setLabelsVisible(true);
  
  QChart* chart = new QChart();
  chart->addSeries(series);
  chart->setTitle(tr("Sample Material Types"));
  
  QBarCategoryAxis* axisX = new QBarCategoryAxis();
  axisX->append(categories);
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);
  
  QValueAxis* axisY = new QValueAxis();
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);
  
  chart->legend()->setVisible(true);
  
  QChartView* chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  return chartView;
}

QChartView* Tab1::createCategoriesChart()
{
  const QMap<QString, int>& categoryCounts = waterModel->getCategoryCounts();
  
  QPieSeries* series = new QPieSeries();
  
  for (auto it = categoryCounts.constBegin(); it != categoryCounts.constEnd(); ++it) {
    QPieSlice* slice = new QPieSlice(it.key(), it.value());
    slice->setLabelVisible(true);
    series->append(slice);
  }
  
  QChart* chart = new QChart();
  chart->addSeries(series);
  chart->setTitle(tr("Pollutant Categories Distribution"));
  chart->legend()->setVisible(true);
  
  QChartView* chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  return chartView;
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

    QLabel#headingLabel {
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