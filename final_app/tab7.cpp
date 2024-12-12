// Implementation of Tab 7 - Help

#include <QtWidgets>
#include "styles.hpp"
#include "tab7.hpp"

Tab7::Tab7(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
  setupStyles();
  
  qApp->installEventFilter(this);
}

bool Tab7::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
    setupStyles();
    return true;
  }
  return QWidget::eventFilter(watched, event);
}

void Tab7::createWidgets()
{
  title = new QLabel(tr("Help"));
  QFont titleFont;
  titleFont.setPointSize(32);
  titleFont.setBold(true);
  title->setFont(titleFont);
  title->setAlignment(Qt::AlignCenter);
  title->setObjectName("titleLabel");

  contentContainer = new QWidget;
  QVBoxLayout* contentLayout = new QVBoxLayout(contentContainer);
  contentLayout->setSpacing(20);
  contentLayout->setContentsMargins(20, 20, 20, 20);

  QFont sectionFont;
  sectionFont.setPointSize(24);
  sectionFont.setBold(true);

  QFont contentFont;
  contentFont.setPointSize(14);

  // About the Dataset
  QLabel* datasetTitle = new QLabel(tr("About the Dataset"));
  datasetTitle->setFont(sectionFont);
  QLabel* datasetText = new QLabel(tr(
    "This application visualises water quality data collected from various locations by the UK Environment Agency. "
    "The datasets includes measurements of different determinands (substances) in water samples, "
    "along with information about sampling locations, dates, and whether samples were taken "
    "to comply with UK regulations."
  ));
  datasetText->setFont(contentFont);
  datasetText->setWordWrap(true);
  contentLayout->addWidget(datasetTitle);
  contentLayout->addWidget(datasetText);

  // Navigation Guide
  QLabel* navTitle = new QLabel(tr("Navigation Guide"));
  navTitle->setFont(sectionFont);
  QLabel* navText = new QLabel(tr(
    "• <b>Dashboard</b>: Overview of key data metrics to quickly assess overall monitoring status<br>"
    "• <b>Pollutant Overview</b>: Track concentrations of key pollutants to monitor water quality and safety<br>"
    "• <b>Litter Indicators</b>: Assess environmental impact and cleanliness of waterways through litter monitoring<br>" 
    "• <b>Fluorinated Compounds</b>: Track levels of fluorinated chemicals to ensure safe drinking water standards and dental health<br>"
    "• <b>Compliance</b>: Monitor adherence to water quality regulations to maintain public safety standards<br>"
    "• <b>Raw Data</b>: Access the complete dataset for detailed research and custom analysis<br>"
    "• <b>Help</b>: Guide to using this water quality monitoring system"
  ));
  navText->setFont(contentFont);
  navText->setTextFormat(Qt::RichText);
  navText->setWordWrap(true);
  contentLayout->addWidget(navTitle);
  contentLayout->addWidget(navText);

  // Using the Application
  QLabel* usageTitle = new QLabel(tr("Using the Application"));
  usageTitle->setFont(sectionFont);
  QLabel* usageText = new QLabel(tr(
    "• Use the tabs at the top to navigate between different pages<br>"
    "• Hover over charts and graphs to see detailed information<br>"
    "• Use the 'Reload Data' on the dashboard or through the file menu to load a different dataset<br>"
    "<br>Data can be downloaded from: "
    "<a href='https://environment.data.gov.uk/water-quality/view/download'>The UK Environment Agency</a><br>"
    "<br>More information about the datasets can be found "
    "<a href='https://environment.data.gov.uk/water-quality/view/doc/reference'>here</a>"
  ));
  usageText->setFont(contentFont);
  usageText->setTextFormat(Qt::RichText);
  usageText->setOpenExternalLinks(true);
  usageText->setWordWrap(true);
  contentLayout->addWidget(usageTitle);
  contentLayout->addWidget(usageText);

  // Compliance Indicators
  QLabel* complianceTitle = new QLabel(tr("Compliance Indicators"));
  complianceTitle->setFont(sectionFont);
  contentLayout->addWidget(complianceTitle);

  // Create colour indicators
  const struct {
    QColor colour;
    QString meaning;
    QString description;
  } indicators[] = {
    {QColor(32, 125, 32), tr("Safe"), tr("Measurement is within safe limits according to the UK Drinking Water Standards")},
    {QColor(253, 183, 0), tr("Warning"), tr("Measurement is approaching threshold (>80%) of the UK Drinking Water Standards")},
    {QColor(204, 31, 40), tr("Exceeded"), tr("Measurement exceeds safe threshold according to the UK Drinking Water Standards")}
  };

  for (const auto& indicator : indicators) {
    QWidget* row = new QWidget;
    QHBoxLayout* rowLayout = new QHBoxLayout(row);
    
    // Colour block
    QFrame* colourBlock = new QFrame;
    colourBlock->setFixedSize(30, 30);
    colourBlock->setStyleSheet(QString("background-color: %1; border-radius: 4px;")
                            .arg(indicator.colour.name()));
    
    // Text
    QLabel* text = new QLabel(QString("<b>%1</b>: %2")
                            .arg(indicator.meaning)
                            .arg(indicator.description));
    text->setFont(contentFont);
    text->setWordWrap(true);
    
    rowLayout->addWidget(colourBlock);
    rowLayout->addWidget(text, 1);
    rowLayout->setSpacing(15);
    
    contentLayout->addWidget(row);
  }

  scrollArea = new QScrollArea;
  scrollArea->setWidget(contentContainer);
  scrollArea->setWidgetResizable(true);
}

void Tab7::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(title);
  layout->addWidget(scrollArea);
  layout->setSpacing(20);
  layout->setContentsMargins(30, 30, 30, 30);
    
  setLayout(layout);
}

void Tab7::setupStyles()
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
      background-color: %2;
      border-radius: 6px;
      min-height: 20px;
      margin: 2px;
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
      background-color: %2;
      border-radius: 6px;
      min-width: 20px;
      margin: 2px;
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
  .arg(Styles::actionButtonHover());           // %2

  this->setStyleSheet(styleSheet);

  // Add tooltip to scrollbar
  scrollArea->verticalScrollBar()->setToolTip(tr("Scroll to view more"));
  scrollArea->horizontalScrollBar()->setToolTip(tr("Scroll to view more"));
}
