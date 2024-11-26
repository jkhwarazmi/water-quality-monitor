// Modified COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include "window.hpp"
#include "loaddatawidget.hpp"
#include "tab1.hpp"
#include "tab2.hpp"
#include "tab3.hpp"


WaterWindow::WaterWindow() : QMainWindow()
{
  // Default data directory  
  dataLocation = "../../data/";
    
  createMainWidget();
  createMenus();
  
  setMinimumWidth(800);
  setMinimumHeight(600);
  setWindowTitle(tr("Water Quality Monitor"));
}

void WaterWindow::createMainWidget()
{
  stackedWidget = new QStackedWidget;

  loadDataWidget = new LoadDataWidget(dataLocation);

  connect(loadDataWidget, SIGNAL(dataFileSelected()), this, SLOT(setDataLocation()));
  connect(loadDataWidget, SIGNAL(loadRequested()), this, SLOT(setPath()));
    
  tabWidget = new QTabWidget;
  tab1 = new Tab1(&model);
  tab2 = new Tab2(&model);
  tab3 = new Tab3(&model);
    
  connect(tab1, SIGNAL(reloadRequested()), this, SLOT(switchToLoadView()));
    
  tabWidget->addTab(tab1, tr("Dashboard"));
  tabWidget->addTab(tab2, tr("Tab 2"));
  tabWidget->addTab(tab3, tr("Raw Data"));
    
  stackedWidget->addWidget(loadDataWidget);
  stackedWidget->addWidget(tabWidget);
    
  setCentralWidget(stackedWidget);
    
  stackedWidget->setCurrentWidget(loadDataWidget);

  connect(loadDataWidget, SIGNAL(languageChanged(QString)), this, SLOT(changeLanguage(QString)));
}

void WaterWindow::createMenus()
{
  QAction *locAction = new QAction(tr("Set Data &Location"), this);
  locAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction, SIGNAL(triggered()), this, SLOT(switchToLoadView()));

  QAction *closeAction = new QAction(tr("Quit"), this);
  closeAction->setShortcut(QKeySequence::Close);
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(locAction);
  fileMenu->addAction(closeAction);

  QAction *aboutAction = new QAction(tr("&About"), this);
  connect(aboutAction, &QAction::triggered, this, &WaterWindow::about);

  QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAction);

  createLanguageMenu();
}

void WaterWindow::createLanguageMenu() 
{
  QMenu *languageMenu = menuBar()->addMenu(tr("&Language"));

  QActionGroup *languageGroup = new QActionGroup(this);
  QAction* englishAction = new QAction(tr("&English"), this);
  englishAction->setCheckable(true);
  englishAction->setData("English");
    
  QAction* frenchAction = new QAction(tr("&Français"), this);
  frenchAction->setCheckable(true);
  frenchAction->setData("French");

  // Set current language
  QString currentLocale = QLocale::system().name().split('_').first();
  if (currentLocale == "fr") {
    frenchAction->setChecked(true);
  } else {
    englishAction->setChecked(true);
  }

  languageGroup->addAction(englishAction);
  languageGroup->addAction(frenchAction);
    
  languageMenu->addAction(englishAction);
  languageMenu->addAction(frenchAction);

  // Connect using a lambda to convert QAction* to QString
  connect(languageGroup, SIGNAL(triggered(QAction *)), this, SLOT(handleLanguageAction(QAction *)));
}

void WaterWindow::handleLanguageAction(QAction *action)
{
  changeLanguage(action->data().toString());
}

void WaterWindow::setDataLocation()
{
  QString directory = QFileDialog::getOpenFileName(this, tr("Select Data File"), dataLocation, tr("CSV files (*.csv)"));

  if (!directory.isEmpty()) {
    dataLocation = directory;
    loadDataWidget->updatePath(dataLocation);
  } else {
    QMessageBox::critical(this, tr("Data Error"), tr("No file selected."));
  }
}

void WaterWindow::setPath()
{
  dataLocation = loadDataWidget->getPath();
  loadData(dataLocation);
}

void WaterWindow::loadData(const QString &path)
{
  QProgressDialog progress(tr("Loading data..."), tr("Cancel"), 0, 0, this);
  progress.setWindowModality(Qt::WindowModal);
  progress.show();

  try {
    model.updateFromFile(path);
      
    if (!model.hasData()) {
      QMessageBox::critical(this, tr("Data Error"), tr("The selected file contains no valid data."));
      return;
    }
        
    switchToTabView();

  } catch (const std::exception& error) {
    QMessageBox::critical(this, tr("CSV File Error"), error.what());
  }
    
  progress.close();
}

void WaterWindow::switchToTabView()
{
  stackedWidget->setCurrentWidget(tabWidget);
}

void WaterWindow::switchToLoadView()
{
  stackedWidget->setCurrentWidget(loadDataWidget);
}

void WaterWindow::changeLanguage(const QString &language)
{ 
  QString code = "en";

  if (language == "French") {
    code = "fr";
  }

  qApp->removeTranslator(&translator);

  if (translator.load("WaterQualityMonitor_" + code, ":/i18n")) {
    qApp->installTranslator(&translator);
  } else {
    QMessageBox::critical(this, tr("Language Error"), tr("Could not load language file."));
  }

  QEvent event(QEvent::LanguageChange);
  qApp->sendEvent(qApp, &event);

  // Not everything translates properly, may need custom changeEvent in each
  // Where we re-specify strings?
  setWindowTitle(tr("Water Quality Monitor"));
}

void WaterWindow::about()
{
  QMessageBox::about(this, tr("About Water Quality Monitor"),
    tr("Water Quality Monitor displays water data loaded from "
    "a CSV file produced by the UK Environment Agency.\n\n"
    "(c) 2024 Nick Efford and Group 37"));
}