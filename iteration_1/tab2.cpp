// Sample implementation for the second card - Tab2

#include <QtWidgets>
#include "tab2.hpp"


Tab2::Tab2(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
}

void Tab2::createWidgets()
{
  placeholder = new QLabel(tr("Coming Soon"));
  QFont labelFont = placeholder->font();
  labelFont.setPointSize(24);
  labelFont.setBold(true);
  placeholder->setFont(labelFont);
  placeholder->setAlignment(Qt::AlignCenter);
}

void Tab2::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(placeholder);
  layout->addStretch();
    
  setLayout(layout);
}