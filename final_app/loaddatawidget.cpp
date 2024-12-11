// Implementation file for the data loading page

#include <QtWidgets>
#include "styles.hpp"
#include "loaddatawidget.hpp"


LoadDataWidget::LoadDataWidget(const QString& initialPath, QWidget* parent): QWidget(parent), currentPath(initialPath)
{
  createWidgets();
  createLayout();
  setupStyles();
  
  qApp->installEventFilter(this);

  // Initially hide the cancel button
  cancelButton->setVisible(false);
}

bool LoadDataWidget::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
    setupStyles();
    return true;
  }
  return QWidget::eventFilter(watched, event);
}

void LoadDataWidget::createWidgets()
{
  titleLabel = new QLabel(tr("Select Data File"));
  QFont titleFont;
  titleFont.setPointSize(24);
  titleFont.setBold(true);
  titleLabel->setFont(titleFont);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setObjectName("titleLabel");
  
  pathLabel = new QLabel(currentPath);
  pathLabel->setWordWrap(true);
  pathLabel->setMinimumWidth(300);
  pathLabel->setObjectName("pathLabel");
  pathLabel->setAutoFillBackground(true);
  pathLabel->setToolTip(tr("Current data file path"));
  
  selectButton = new QPushButton(tr("Select File"));
  selectButton->setMinimumWidth(120);
  selectButton->setMinimumHeight(40);
  selectButton->setCursor(Qt::PointingHandCursor);
  selectButton->setObjectName("primaryButton");
  selectButton->setToolTip(tr("Choose a CSV file to load"));

  loadButton = new QPushButton(tr("Load Data"));
  loadButton->setMinimumWidth(200);
  loadButton->setMinimumHeight(45);
  loadButton->setCursor(Qt::PointingHandCursor);
  loadButton->setObjectName("actionButton");
  loadButton->setEnabled(currentPath.endsWith(".csv", Qt::CaseInsensitive));
  loadButton->setToolTip(loadButton->isEnabled() ? 
    tr("Load the selected data file") : 
    tr("No data file selected"));

  cancelButton = new QPushButton(tr("Cancel Reload"));
  cancelButton->setMinimumWidth(120);
  cancelButton->setMinimumHeight(45);
  cancelButton->setCursor(Qt::PointingHandCursor);
  cancelButton->setObjectName("secondaryButton");
  cancelButton->setToolTip(tr("Return to viewing current data"));
  cancelButton->hide();

  helpLabel = new QLabel(tr("Load water quality data from a CSV file. Datasets can be downloaded from:"));
  helpLabel->setWordWrap(true);
  helpLabel->setObjectName("helpLabel");
  helpLabel->setAlignment(Qt::AlignCenter);
  
  urlLabel = new QLabel(tr("<a href='https://environment.data.gov.uk/water-quality/view/download'>The UK Environment Agency</a>"));
  urlLabel->setTextFormat(Qt::RichText);
  urlLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
  urlLabel->setOpenExternalLinks(true);
  urlLabel->setObjectName("urlLabel");
  urlLabel->setAlignment(Qt::AlignCenter);
  urlLabel->setCursor(Qt::PointingHandCursor);
  urlLabel->setToolTip(tr("Click to download sample datasets"));

  infoLabel = new QLabel(tr("More information on the datasets can be found <a href='https://environment.data.gov.uk/water-quality/view/doc/reference'>here</a>"));
  infoLabel->setTextFormat(Qt::RichText);
  infoLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
  infoLabel->setOpenExternalLinks(true);
  infoLabel->setObjectName("helpLabel");
  infoLabel->setAlignment(Qt::AlignCenter);
  infoLabel->setCursor(Qt::PointingHandCursor);
  infoLabel->setToolTip(tr("Click for more information about the datasets"));
}

void LoadDataWidget::createLayout()
{
  // File selection area layout
  QHBoxLayout* pathLayout = new QHBoxLayout;
  pathLayout->addWidget(pathLabel, 1);
  pathLayout->addWidget(selectButton);
  pathLayout->setSpacing(15);
  pathLayout->setContentsMargins(0, 0, 0, 0);

  // Button area layout
  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->addStretch();
  buttonLayout->addWidget(cancelButton);
  buttonLayout->addWidget(loadButton);
  buttonLayout->setSpacing(15);
  buttonLayout->setContentsMargins(0, 0, 0, 0);

  // Main layout
  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(titleLabel);
  mainLayout->addSpacing(10);
  mainLayout->addWidget(helpLabel);
  mainLayout->addWidget(urlLabel);
  mainLayout->addSpacing(5);
  mainLayout->addWidget(infoLabel);
  mainLayout->addSpacing(30);
  mainLayout->addLayout(pathLayout);
  mainLayout->addSpacing(20);
  mainLayout->addLayout(buttonLayout);
  mainLayout->addStretch();

  connect(selectButton, SIGNAL(clicked()), this, SIGNAL(dataFileSelected()));
  connect(loadButton, SIGNAL(clicked()), this, SIGNAL(loadRequested()));
  connect(cancelButton, SIGNAL(clicked()), this, SIGNAL(cancelRequested()));
  
  setLayout(mainLayout);
}

void LoadDataWidget::setDataLoaded(bool loaded)
{
  dataLoaded = loaded;
  cancelButton->setVisible(dataLoaded);
}

void LoadDataWidget::updatePath(const QString& path)
{
  currentPath = path;
  pathLabel->setText(path);
  bool isEnabled = currentPath.endsWith(".csv", Qt::CaseInsensitive);
  loadButton->setEnabled(isEnabled);
  loadButton->setToolTip(isEnabled ? 
    tr("Load the selected data file") : 
    tr("No data file selected"));
}

void LoadDataWidget::setupStyles()
{
  QString styleSheet = QString(R"(
    QWidget {
      background-color: transparent;
      color: %1;
    }

    QLabel#titleLabel, QLabel#pathLabel {
      color: %1;
    }

    QLabel#pathLabel {
      background-color: %2;
      border-radius: %3px;
      padding: %4px;
    }

    QPushButton#primaryButton {
      background-color: %5;
      border: 2px solid %6;
      border-radius: %3px;
      color: %1;
      font-weight: bold;
      padding: %7px %8px;
      min-width: %9px;
      min-height: %10px;
    }

    QPushButton#primaryButton:hover {
      background-color: %11;
    }

    QPushButton#actionButton {
      background-color: %12;
      border: none;
      border-radius: %3px;
      color: white;
      font-weight: bold;
      padding: %7px %8px;
      min-width: %13px;
      min-height: %14px;
    }

    QPushButton#actionButton:hover {
      background-color: %15;
    }

    QPushButton#actionButton:disabled {
      background-color: %16;
      color: white;
    }

    QPushButton#secondaryButton {
      background-color: #ff4d4d;
      border: none;
      border-radius: %3px;
      color: white;
      font-weight: bold;
      padding: %7px %8px;
      min-width: 120px;
      min-height: 40px;
    }

    QPushButton#secondaryButton:hover {
      background-color: #ff6666;
    }

    QLabel#helpLabel {
      color: %1;
      font-size: 14px;
    }

    QLabel#urlLabel {
      color: #3498db;
      font-size: 14px;
    }

    QLabel#urlLabel:hover {
      color: #2980b9;
    }
  )")
  .arg(Styles::textColor())                    // %1
  .arg(Styles::primaryButtonBackground())      // %2
  .arg(Styles::BORDER_RADIUS)                  // %3
  .arg(Styles::PADDING_MEDIUM)                 // %4
  .arg(Styles::primaryButtonBackground())      // %5
  .arg(Styles::primaryButtonBorder())          // %6
  .arg(Styles::PADDING_MEDIUM)                 // %7
  .arg(Styles::PADDING_LARGE)                  // %8
  .arg(Styles::BUTTON_MIN_WIDTH)               // %9
  .arg(Styles::BUTTON_MIN_HEIGHT)              // %10
  .arg(Styles::primaryButtonHover())           // %11
  .arg(Styles::actionButtonBackground())       // %12
  .arg(Styles::ACTION_BUTTON_WIDTH)            // %13
  .arg(Styles::ACTION_BUTTON_HEIGHT)           // %14
  .arg(Styles::actionButtonHover())            // %15
  .arg(Styles::actionButtonDisabled());        // %16

  this->setStyleSheet(styleSheet);
}
