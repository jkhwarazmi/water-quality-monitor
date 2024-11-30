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

  chartView = new QChartView();
  chartView->setRenderHint(QPainter::Antialiasing);
  chartView->setMinimumSize(400, 300);

  // Create charts
  updateCharts();
}

void Tab1::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(chartView);
  layout->addStretch();
  layout->addWidget(reloadButton, 0, Qt::AlignCenter);
  layout->setSpacing(20);
  layout->setContentsMargins(30, 30, 30, 30);
    
  setLayout(layout);
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

    QLabel#placeholderLabel {
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

void Tab1::updateCharts() {
  updateComplianceChart();
}

void Tab1::updateComplianceChart() {
  // Get compliance statistics from model
  QMap<QString, int> stats = waterModel->getComplianceStats();
  
  // Create pie series
  QPieSeries *series = new QPieSeries();
  
  // Add slices
  series->append("Compliance", stats["Compliance"]);
  series->append("Non-Compliance", stats["Non-Compliance"]);
  
  // Customize slices
  QPieSlice *compliantSlice = series->slices().at(0);
  compliantSlice->setBrush(QColor("#4CAF50")); // Green
  compliantSlice->setLabelVisible(true);
  
  QPieSlice *nonCompliantSlice = series->slices().at(1);
  nonCompliantSlice->setBrush(QColor("#F44336")); // Red
  nonCompliantSlice->setLabelVisible(true);
  
  // Create chart
  QChart *chart = new QChart();
  chart->addSeries(series);
  chart->setTitle("Compliance Distribution");
  chart->legend()->setVisible(true);
  chart->legend()->setAlignment(Qt::AlignBottom);
  
  // Set chart to chart view
  chartView->setChart(chart);
}