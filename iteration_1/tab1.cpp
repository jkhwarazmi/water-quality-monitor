// Sample implementation for the first card - Tab1

#include <QtWidgets>
#include "tab1.hpp"


Tab1::Tab1(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
}

void Tab1::createWidgets()
{
  reloadButton = new QPushButton(tr("Reload Data"));
  connect(reloadButton, SIGNAL(clicked()), this, SIGNAL(reloadRequested()));

  placeholder = new QLabel(tr("Coming Soon"));
  QFont labelFont = placeholder->font();
  labelFont.setPointSize(24);
  labelFont.setBold(true);
  placeholder->setFont(labelFont);
  placeholder->setAlignment(Qt::AlignCenter);
}

void Tab1::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(placeholder);
  layout->addStretch();
  layout->addWidget(reloadButton);
    
  setLayout(layout);
}