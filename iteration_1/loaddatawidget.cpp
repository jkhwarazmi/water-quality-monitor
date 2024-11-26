// Implementation file for the data loading page

#include <QtWidgets>
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
  bool isDarkMode = qApp->palette().color(QPalette::Window).lightness() < 128;
  
  // Define colors based on theme
  QString textColor = isDarkMode ? "#ffffff" : "#2c3e50";
  QString bgColor = isDarkMode ? "#2c3e50" : "#ffffff";
  QString pathBgColor = isDarkMode ? "#34495e" : "#f8f9fa";
  QString primaryButtonBg = isDarkMode ? "#34495e" : "#f8f9fa";
  QString primaryButtonBorder = isDarkMode ? "#4a6480" : "#e9ecef";
  QString primaryButtonHover = isDarkMode ? "#4a6480" : "#e9ecef";
  QString actionButtonBg = isDarkMode ? "#3498db" : "#007bff";
  QString actionButtonHover = isDarkMode ? "#2980b9" : "#0056b3";
  QString actionButtonDisabled = isDarkMode ? "#445566" : "#cccccc";
  QString secondaryButtonBg = isDarkMode ? "#95a5a6" : "#6c757d";
  QString secondaryButtonHover = isDarkMode ? "#7f8c8d" : "#5a6268";
  QString comboBoxBg = isDarkMode ? "#34495e" : "#ffffff";
  QString comboBoxBorder = isDarkMode ? "#4a6480" : "#e9ecef";
  QString comboBoxText = isDarkMode ? "#ffffff" : "#2c3e50";
  QString comboBoxArrow = isDarkMode ? "#ecf0f1" : "#2c3e50";
  
  QString styleSheet = QString(R"(
    QWidget {
      background-color: transparent;
      color: %1;
    }

    QLabel#pathLabel {
      background-color: %2;
      border-radius: 6px;
      padding: 8px;
    }

    QPushButton#primaryButton {
      background-color: %3;
      border: 2px solid %4;
      border-radius: 6px;
      color: %1;
      font-weight: bold;
      padding: 8px 16px;
    }

    QPushButton#primaryButton:hover {
      background-color: %5;
    }

    QPushButton#actionButton {
      background-color: %6;
      border: none;
      border-radius: 6px;
      color: white;
      font-weight: bold;
      padding: 10px 20px;
    }

    QPushButton#actionButton:hover {
      background-color: %7;
    }

    QPushButton#actionButton:disabled {
      background-color: %8;
      color: white;
    }

    QPushButton#secondaryButton {
      background-color: %9;
      border: none;
      border-radius: 6px;
      color: white;
      padding: 8px 16px;
    }

    QPushButton#secondaryButton:hover {
      background-color: %10;
    }

    QComboBox#languageSelect {
      background-color: %11;
      border: 2px solid %12;
      border-radius: 6px;
      color: %13;
      padding: 5px 10px;
    }

    QComboBox#languageSelect::drop-down {
      border: none;
      width: 30px;
    }

    QComboBox#languageSelect::down-arrow {
      image: none;
      border-left: 5px solid transparent;
      border-right: 5px solid transparent;
      border-top: 5px solid %14;
      margin-right: 10px;
    }

    QComboBox#languageSelect QAbstractItemView {
      background-color: %11;
      border: 2px solid %12;
      color: %13;
      selection-background-color: %5;
    }
  )")
  .arg(textColor)         // %1 text color
  .arg(pathBgColor)       // %2 path background
  .arg(primaryButtonBg)   // %3
  .arg(primaryButtonBorder) // %4
  .arg(primaryButtonHover)  // %5
  .arg(actionButtonBg)      // %6
  .arg(actionButtonHover)   // %7
  .arg(actionButtonDisabled) // %8
  .arg(secondaryButtonBg)    // %9
  .arg(secondaryButtonHover) // %10
  .arg(comboBoxBg)          // %11
  .arg(comboBoxBorder)      // %12
  .arg(comboBoxText)        // %13
  .arg(comboBoxArrow);      // %14

  this->setStyleSheet(styleSheet);
}

void LoadDataWidget::updatePath(const QString& path)
{
  currentPath = path;
  pathLabel->setText(path);
  loadButton->setEnabled(currentPath.endsWith(".csv", Qt::CaseInsensitive));
}
