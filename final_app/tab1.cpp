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
  complianceChartView->setToolTip(tr("Number of samples taken to measure compliance with regulations compared to samples taken for other reasons"));
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

void Tab1::updateCharts()
{
  updateStats();
  updateComplianceChart();
  updateLocationChart();
  updateDeterminandChart();
  updateTimelineChart();
}

void Tab1::updateStats()
{
  stats->setText(
    tr("Distinct Samples Taken: ") + QString("%L1").arg(waterModel->rowCount(QModelIndex())) + "\n" +
    tr("Distinct Locations Measured: ") + QString("%L1").arg(waterModel->getLocationCount()) + "\n" +
    tr("Distinct Determinands Sampled: ") + QString("%L1").arg(waterModel->getDeterminandCount()) + "\n\n" +
    tr("Last Updated: ") + QDateTime::currentDateTime().toString("yyyy-MM-dd, hh:mm:ss")
  );
}

void Tab1::updateComplianceChart()
{
  // No tooltips here, hover gives chart details
  complianceSeries->clear();
  QPair<int, int> complianceData = waterModel->getComplianceSampleCount();
  int totalSamples = complianceData.first + complianceData.second;
  
  if (totalSamples > 0) {
    double compliancePercent = (complianceData.first * 100.0) / totalSamples;
    double otherPercent = (complianceData.second * 100.0) / totalSamples;
    
    complianceSeries->append(tr("Compliance (%1%)").arg(QString::number(compliancePercent, 'f', 1)), 
                            complianceData.first);
    complianceSeries->append(tr("Other Samples (%1%)").arg(QString::number(otherPercent, 'f', 1)), 
                            complianceData.second);
    complianceSeries->setHoleSize(0.35);
  }
}

void Tab1::updateLocationChart() {
  locationSeries->clear();
  QMap<QString, int> locationData = waterModel->getSampleLocations();
  
  QBarSet *set = new QBarSet("Samples");
  QStringList categories;
  
  QVector<QPair<QString, int>> sortedLocations;
  for(auto it = locationData.constBegin(); it != locationData.constEnd(); ++it) {
    sortedLocations.append(qMakePair(it.key(), it.value()));
  }
  
  std::sort(sortedLocations.begin(), sortedLocations.end(),
    [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
      return a.second > b.second;
  });
  
  for(const auto& pair : sortedLocations) {
    categories << pair.first;
    *set << pair.second;
  }
  
  locationSeries->append(set);
  
  // Tooltips on hover
  connect(locationSeries, &QBarSeries::hovered,
  [this, categories](bool status, int index, QBarSet *barset) {
    if (status && index >= 0 && index < categories.size()) {
      QString tooltipText = QString("%1\nSamples: %2")
        .arg(categories[index])
        .arg(barset->at(index));
          
      QToolTip::showText(
        QCursor::pos(),
        tooltipText,
        locationChartView
      );
      } else {
        QToolTip::hideText();
      }
  });
  
  QBarCategoryAxis *axis = qobject_cast<QBarCategoryAxis*>(
    locationChartView->chart()->axes(Qt::Horizontal).first()
  );
  axis->clear();
  axis->append(categories);
  
  QValueAxis *valueAxis = qobject_cast<QValueAxis*>(
    locationChartView->chart()->axes(Qt::Vertical).first()
  );
  int maxValue = sortedLocations.isEmpty() ? 0 : sortedLocations.first().second;
  valueAxis->setRange(0, maxValue * 1.1);
  valueAxis->setLabelFormat("%d");
}

void Tab1::updateDeterminandChart() {
  determinandSeries->clear();
  QMap<QString, int> determinandData = waterModel->getDeterminandCounts();
  
  QBarSet *set = new QBarSet("Samples");
  QStringList categories;
  
  QVector<QPair<QString, int>> sortedDeterminands;
  for(auto it = determinandData.constBegin(); it != determinandData.constEnd(); ++it) {
    sortedDeterminands.append(qMakePair(it.key(), it.value()));
  }
  
  std::sort(sortedDeterminands.begin(), sortedDeterminands.end(),
    [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
      return a.second > b.second;
  });
  
  for(const auto& pair : sortedDeterminands) {
    categories << pair.first;
    *set << pair.second;
  }
  
  determinandSeries->append(set);
  
  // Add tooltip handling
  connect(determinandSeries, &QBarSeries::hovered,
    [this, categories](bool status, int index, QBarSet *barset) {
      if (status && index >= 0 && index < categories.size()) {
        QString tooltipText = QString("%1\nSamples: %2")
          .arg(categories[index])
          .arg(barset->at(index));
        
        QToolTip::showText(
          QCursor::pos(),
          tooltipText,
          determinandChartView
        );
      } else {
        QToolTip::hideText();
      }
  });
  
  QBarCategoryAxis *axis = qobject_cast<QBarCategoryAxis*>(
    determinandChartView->chart()->axes(Qt::Horizontal).first()
  );
  axis->clear();
  axis->append(categories);
  
  QValueAxis *valueAxis = qobject_cast<QValueAxis*>(
    determinandChartView->chart()->axes(Qt::Vertical).first()
  );
  int maxValue = sortedDeterminands.isEmpty() ? 0 : sortedDeterminands.first().second;
  valueAxis->setRange(0, maxValue * 1.1);
  valueAxis->setLabelFormat("%d");
}

void Tab1::updateTimelineChart() {
  timelineSeries->clear();
  QMap<QDate, int> dateData = waterModel->getSampleDates();
  
  for(auto it = dateData.constBegin(); it != dateData.constEnd(); ++it) {
    qint64 msecsSinceEpoch = it.key().startOfDay().toMSecsSinceEpoch();
    timelineSeries->append(msecsSinceEpoch, it.value());
  }
  
  if(!dateData.isEmpty()) {
    connect(timelineSeries, &QLineSeries::hovered,
      [](const QPointF &point, bool status) {
        if (status) {
          QDateTime pointDate = QDateTime::fromMSecsSinceEpoch(point.x());
          QString tooltipText = QString("%1\nSamples: %2")
            .arg(pointDate.toString("yyyy-MM-dd"))
            .arg(point.y(), 0, 'f', 0);
          
          QToolTip::showText(
            QCursor::pos(),
            tooltipText
          );
        } else {
          QToolTip::hideText();
        }
    });
    
    QDateTimeAxis *axisX = qobject_cast<QDateTimeAxis*>(
      timelineChartView->chart()->axes(Qt::Horizontal).first()
    );
    axisX->setRange(dateData.firstKey().startOfDay(), dateData.lastKey().endOfDay());
    
    QValueAxis *axisY = qobject_cast<QValueAxis*>(
      timelineChartView->chart()->axes(Qt::Vertical).first()
    );
    int maxValue = *std::max_element(dateData.begin(), dateData.end());
    
    axisY->setRange(0, maxValue * 1.2);
    
    int tickCount = std::min(10, maxValue + 1);
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

    QScrollArea {
      border: none;
      background-color: transparent;
    }

    QScrollBar:vertical {
      background-color: transparent;
      width: 12px;
      margin: 0;
    }

    QScrollBar::handle:vertical {
      background-color: %10;
      border-radius: 6px;
      min-height: 20px;
      margin: 2px;
    }

    QScrollBar::handle:vertical:hover {
      background-color: %8;
    }

    QScrollBar::add-line:vertical,
    QScrollBar::sub-line:vertical {
      height: 0px;
    }

    QScrollBar::add-page:vertical,
    QScrollBar::sub-page:vertical {
      background: none;
    }

    QScrollBar:horizontal {
      background-color: transparent;
      height: 12px;
      margin: 0;
    }

    QScrollBar::handle:horizontal {
      background-color: %10;
      border-radius: 6px;
      min-width: 20px;
      margin: 2px;
    }

    QScrollBar::handle:horizontal:hover {
      background-color: %8;
    }

    QScrollBar::add-line:horizontal,
    QScrollBar::sub-line:horizontal {
      width: 0px;
    }

    QScrollBar::add-page:horizontal,
    QScrollBar::sub-page:horizontal {
      background: none;
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
  .arg(Styles::actionButtonDisabled())         // %9
  .arg(Styles::borderColor());                 // %10

  this->setStyleSheet(styleSheet);
}
