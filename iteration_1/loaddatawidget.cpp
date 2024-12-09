// Implementation file for the data loading page

#include <QtWidgets>
#include "loaddatawidget.hpp"


LoadDataWidget::LoadDataWidget(const QString& initialPath, QWidget* parent): QWidget(parent), currentPath(initialPath)
{
  createWidgets();
  createLayout();
}

void LoadDataWidget::createWidgets()
{
  titleLabel = new QLabel(tr("Select Data File"));
  QFont titleFont = titleLabel->font();
  titleFont.setPointSize(16);
  titleFont.setBold(true);
  titleLabel->setFont(titleFont);
  titleLabel->setAlignment(Qt::AlignCenter);

  pathLabel = new QLabel(currentPath);
  selectButton = new QPushButton(tr("Select File"));

  loadButton = new QPushButton(tr("Load Data"));
  loadButton->setEnabled(currentPath.endsWith(".csv", Qt::CaseInsensitive));

  cancelButton = new QPushButton(tr("Cancel"));
  cancelButton->hide();
}

void LoadDataWidget::createLayout()
{
  QHBoxLayout *pathLayout = new QHBoxLayout;
  pathLayout->addWidget(pathLabel);
  pathLayout->addWidget(selectButton);
    
  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(titleLabel);
  mainLayout->addSpacing(20);
  mainLayout->addLayout(pathLayout);
  mainLayout->addWidget(loadButton);
  mainLayout->addWidget(cancelButton);
  mainLayout->addStretch();

  connect(selectButton, SIGNAL(clicked()), this, SIGNAL(dataFileSelected()));
  connect(loadButton, SIGNAL(clicked()), this, SIGNAL(loadRequested()));
  
  setLayout(mainLayout);
}

void LoadDataWidget::updatePath(const QString& path)
{
  currentPath = path;
  pathLabel->setText(path);
  loadButton->setEnabled(currentPath.endsWith(".csv", Qt::CaseInsensitive));
}
