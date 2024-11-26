// Sample implementation for the second card - Tab2

#include <QtWidgets>
#include "tab2.hpp"


Tab2::Tab2(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
  setupStyles();
  
  // Install event filter for theme changes
  qApp->installEventFilter(this);
}

bool Tab2::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
    setupStyles();
    return true;
  }
  return QWidget::eventFilter(watched, event);
}

void Tab2::createWidgets()
{
  placeholder = new QLabel(tr("Coming Soon"));
  QFont labelFont;
  labelFont.setPointSize(32);
  labelFont.setBold(true);
  placeholder->setFont(labelFont);
  placeholder->setAlignment(Qt::AlignCenter);
  placeholder->setObjectName("placeholderLabel");
}

void Tab2::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(placeholder);
  layout->addStretch();
  layout->setSpacing(20);
  layout->setContentsMargins(30, 30, 30, 30);
    
  setLayout(layout);
}

void Tab2::setupStyles()
{
  bool isDarkMode = qApp->palette().color(QPalette::Window).lightness() < 128;
  
  // Define colors based on theme
  QString textColor = isDarkMode ? "#ffffff" : "#2c3e50";
  
  QString styleSheet = QString(R"(
    QWidget {
      background-color: transparent;
      color: %1;
    }

    QLabel#placeholderLabel {
      font-size: 32px;
      font-weight: bold;
    }
  )")
  .arg(textColor);  // %1 text color

  this->setStyleSheet(styleSheet);
}
