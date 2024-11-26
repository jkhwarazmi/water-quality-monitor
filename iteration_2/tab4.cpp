// Sample implementation for the third card - Tab4

#include <QtWidgets>
#include "styles.hpp"
#include "tab4.hpp"


Tab4::Tab4(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
  setupStyles();
  
  // Install event filter for theme changes
  qApp->installEventFilter(this);
}

bool Tab4::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
    setupStyles();
    return true;
  }
  return QWidget::eventFilter(watched, event);
}

void Tab4::createWidgets()
{
  placeholder = new QLabel(tr("Coming Soon"));
  QFont labelFont;
  labelFont.setPointSize(32);
  labelFont.setBold(true);
  placeholder->setFont(labelFont);
  placeholder->setAlignment(Qt::AlignCenter);
  placeholder->setObjectName("placeholderLabel");
}

void Tab4::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(placeholder);
  layout->addStretch();
  layout->setSpacing(20);
  layout->setContentsMargins(30, 30, 30, 30);
    
  setLayout(layout);
}

void Tab4::setupStyles()
{
  QString styleSheet = QString(R"(
    QWidget {
      background-color: transparent;
      color: %1;
    }

    QLabel#placeholderLabel {
      font-size: 32px;
      font-weight: bold;
      color: %1;
    }
  )")
  .arg(Styles::textColor());        // %1

  this->setStyleSheet(styleSheet);
}
