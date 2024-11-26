// Sample implementation for the first card - Tab1

#include <QtWidgets>
#include "tab1.hpp"


Tab1::Tab1(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
  setupStyles();
  
  // Install event filter for theme changes
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

  placeholder = new QLabel(tr("Coming Soon"));
  QFont labelFont;
  labelFont.setPointSize(32);
  labelFont.setBold(true);
  placeholder->setFont(labelFont);
  placeholder->setAlignment(Qt::AlignCenter);
  placeholder->setObjectName("placeholderLabel");
}

void Tab1::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(placeholder);
  layout->addStretch();
  layout->addWidget(reloadButton, 0, Qt::AlignCenter);
  layout->setSpacing(20);
  layout->setContentsMargins(30, 30, 30, 30);
    
  setLayout(layout);
}

void Tab1::setupStyles()
{
  bool isDarkMode = qApp->palette().color(QPalette::Window).lightness() < 128;
  
  // Define colors based on theme
  QString textColor = isDarkMode ? "#ffffff" : "#2c3e50";
  QString actionButtonBg = isDarkMode ? "#3498db" : "#007bff";
  QString actionButtonHover = isDarkMode ? "#2980b9" : "#0056b3";
  QString actionButtonDisabled = isDarkMode ? "#445566" : "#cccccc";
  
  QString styleSheet = QString(R"(
    QWidget {
      background-color: transparent;
      color: %1;
    }

    QPushButton#actionButton {
      background-color: %2;
      border: none;
      border-radius: 6px;
      color: white;
      font-weight: bold;
      padding: 10px 20px;
    }

    QPushButton#actionButton:hover {
      background-color: %3;
    }

    QPushButton#actionButton:disabled {
      background-color: %4;
      color: white;
    }

    QLabel#placeholderLabel {
      font-size: 32px;
      font-weight: bold;
    }
  )")
  .arg(textColor)            // %1 text color
  .arg(actionButtonBg)       // %2
  .arg(actionButtonHover)    // %3
  .arg(actionButtonDisabled); // %4

  this->setStyleSheet(styleSheet);
}
