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

  languageLabel = new QLabel(tr("Language:"));
  languageComboBox = new QComboBox();
  languageComboBox->addItem("English", "en");
  languageComboBox->addItem("French", "fr");

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
  QHBoxLayout *pathLayout = new QHBoxLayout;
  pathLayout->addWidget(pathLabel);
  pathLayout->addWidget(selectButton);

  QFormLayout *languageLayout = new QFormLayout;
  languageLayout->addRow(languageLabel, languageComboBox);
    
  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(titleLabel);
  mainLayout->addSpacing(20);
  mainLayout->addLayout(pathLayout);
  mainLayout->addWidget(loadButton);
  mainLayout->addWidget(cancelButton);
  mainLayout->addStretch();
  mainLayout->addLayout(languageLayout);

  connect(selectButton, SIGNAL(clicked()), this, SIGNAL(dataFileSelected()));
  connect(loadButton, SIGNAL(clicked()), this, SIGNAL(loadRequested()));
  connect(languageComboBox, SIGNAL(currentTextChanged(QString)), this, SIGNAL(changeLanguage(QString)));
  
  setLayout(mainLayout);
}

void LoadDataWidget::updatePath(const QString& path)
{
  currentPath = path;
  pathLabel->setText(path);
  loadButton->setEnabled(currentPath.endsWith(".csv", Qt::CaseInsensitive));
}
