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
  
  selectButton = new QPushButton(tr("Select File"));
  selectButton->setMinimumWidth(120);
  selectButton->setMinimumHeight(40);
  selectButton->setCursor(Qt::PointingHandCursor);
  selectButton->setObjectName("primaryButton");

  loadButton = new QPushButton(tr("Load Data"));
  loadButton->setMinimumWidth(200);
  loadButton->setMinimumHeight(45);
  loadButton->setCursor(Qt::PointingHandCursor);
  loadButton->setObjectName("actionButton");
  loadButton->setEnabled(currentPath.endsWith(".csv", Qt::CaseInsensitive));

  cancelButton = new QPushButton(tr("Cancel"));
  cancelButton->setMinimumWidth(120);
  cancelButton->setMinimumHeight(40);
  cancelButton->setCursor(Qt::PointingHandCursor);
  cancelButton->setObjectName("secondaryButton");
  cancelButton->hide();

  languageLabel = new QLabel(tr("Language:"));
  languageLabel->setObjectName("settingsLabel");
  
  languageComboBox = new QComboBox();
  languageComboBox->setMinimumWidth(150);
  languageComboBox->setMinimumHeight(35);
  languageComboBox->addItem("English", "en");
  languageComboBox->addItem("French", "fr");
  languageComboBox->setObjectName("languageSelect");

  // Set initial language based on current locale
  QString currentLocale = QLocale::system().name().split('_').first();
  int index = languageComboBox->findData(currentLocale);
  if (index != -1) {
    languageComboBox->setCurrentIndex(index);
  } else {
    languageComboBox->setCurrentIndex(0);
  }
}

void LoadDataWidget::createLayout()
{
  // File selection area layout
  QHBoxLayout* pathLayout = new QHBoxLayout;
  pathLayout->addWidget(pathLabel, 1);
  pathLayout->addWidget(selectButton);
  pathLayout->setSpacing(15);
  pathLayout->setContentsMargins(0, 0, 0, 0);

  // Language selection area
  QHBoxLayout* languageLayout = new QHBoxLayout;
  languageLayout->addWidget(languageLabel);
  languageLayout->addWidget(languageComboBox);
  languageLayout->addStretch();
  languageLayout->setSpacing(10);
  languageLayout->setContentsMargins(0, 0, 0, 0);

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
  mainLayout->addSpacing(30);
  mainLayout->addLayout(pathLayout);
  mainLayout->addSpacing(20);
  mainLayout->addLayout(buttonLayout);
  mainLayout->addStretch();
  mainLayout->addLayout(languageLayout);
  mainLayout->setSpacing(10);
  mainLayout->setContentsMargins(30, 30, 30, 30);

  connect(selectButton, SIGNAL(clicked()), this, SIGNAL(dataFileSelected()));
  connect(loadButton, SIGNAL(clicked()), this, SIGNAL(loadRequested()));
  connect(languageComboBox, SIGNAL(currentTextChanged(QString)), this, SIGNAL(languageChanged(QString)));
  
  setLayout(mainLayout);
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
      background-color: %17;
      border: none;
      border-radius: %3px;
      color: white;
      padding: %7px %8px;
      min-width: %9px;
      min-height: %10px;
    }

    QPushButton#secondaryButton:hover {
      background-color: %18;
    }

    QComboBox#languageSelect {
      background-color: %5;
      border: 2px solid %6;
      border-radius: %3px;
      color: %1;
      padding: 5px 10px;
      min-width: 150px;
      min-height: 35px;
    }

    QComboBox#languageSelect::drop-down {
      border: none;
      width: 30px;
    }

    QComboBox#languageSelect::down-arrow {
      image: none;
      border-left: 5px solid transparent;
      border-right: 5px solid transparent;
      border-top: 5px solid %1;
      margin-right: 10px;
    }

    QComboBox#languageSelect QAbstractItemView {
      background-color: %5;
      border: 2px solid %6;
      color: %1;
      selection-background-color: %11;
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
  .arg(Styles::actionButtonDisabled())         // %16
  .arg(Styles::secondaryButtonBackground())    // %17
  .arg(Styles::secondaryButtonHover());        // %18

  this->setStyleSheet(styleSheet);
}

void LoadDataWidget::updatePath(const QString& path)
{
  currentPath = path;
  pathLabel->setText(path);
  loadButton->setEnabled(currentPath.endsWith(".csv", Qt::CaseInsensitive));
}
