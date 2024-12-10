// Modified COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include "styles.hpp"
#include "window.hpp"
#include "loaddatawidget.hpp"
#include "tab1.hpp"
#include "tab2.hpp"
#include "tab3.hpp"
#include "tab4.hpp"
#include "tab5.hpp"
#include "tab6.hpp"
#include "tab7.hpp"

WaterWindow::WaterWindow() : QMainWindow()
{
  // Default data directory  
  dataLocation = "../../data/";
    
  createMainWidget();
  createMenus();
  setupStyles();
  createStatusBar();
  setMinimumWidth(1250);
  setMinimumHeight(600);
  setWindowTitle(tr("Water Quality Monitor"));

  // Install event filter for Styles changes
  qApp->installEventFilter(this);
}

bool WaterWindow::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
    setupStyles();
    return true;
  }
  return QMainWindow::eventFilter(watched, event);
}

void WaterWindow::createMainWidget()
{
  stackedWidget = new QStackedWidget;
  stackedWidget->setObjectName("mainStack");

  loadDataWidget = new LoadDataWidget(dataLocation);

  connect(loadDataWidget, SIGNAL(dataFileSelected()), this, SLOT(setDataLocation()));
  connect(loadDataWidget, SIGNAL(loadRequested()), this, SLOT(setPath()));
  connect(loadDataWidget, SIGNAL(languageChanged(QString)), this, SLOT(changeLanguage(QString)));
  connect(loadDataWidget, SIGNAL(cancelRequested()), this, SLOT(cancelLoad()));
    
  tabWidget = new QTabWidget;
  tabWidget->setObjectName("mainTabs");
  
  tab1 = new Tab1(&model);
  tab2 = new Tab2(&model);
  tab3 = new Tab3(&model);
  tab4 = new Tab4(&model);
  tab5 = new Tab5(&model);
  tab6 = new Tab6(&model);
  tab7 = new Tab7(&model);
    
  connect(tab1, SIGNAL(reloadRequested()), this, SLOT(switchToLoadView()));
    
  tabWidget->addTab(tab1, tr("Dashboard"));
  tabWidget->setTabToolTip(0, tr("View summary of the loaded data"));

  tabWidget->addTab(tab2, tr("Pollutant Overview"));
  tabWidget->setTabToolTip(1, tr("Analyse various water pollutants"));

  tabWidget->addTab(tab3, tr("Litter Indicators"));
  tabWidget->setTabToolTip(2, tr("Monitor water litter levels"));

  tabWidget->addTab(tab4, tr("Fluorinated Compounds"));
  tabWidget->setTabToolTip(3, tr("Examine fluorinated compound data"));

  tabWidget->addTab(tab5, tr("Compliance"));
  tabWidget->setTabToolTip(4, tr("Check compliance of detected pollutants with UK regulations"));

  tabWidget->addTab(tab6, tr("Hotspots"));
  tabWidget->setTabToolTip(5, tr("Identify pollution hotspots"));

  tabWidget->addTab(tab7, tr("Raw Data"));
  tabWidget->setTabToolTip(6, tr("View data from the CSV file as a table"));
    
  stackedWidget->addWidget(loadDataWidget);
  stackedWidget->addWidget(tabWidget);
    
  setCentralWidget(stackedWidget);
    
  stackedWidget->setCurrentWidget(loadDataWidget);
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
  fileInfo->setText("Current file: " + dataLocation);
}

void WaterWindow::cancelLoad()
{
  switchToTabView();
}

void WaterWindow::loadData(const QString &path)
{
  // Create custom loading dialog
  QDialog loadingDialog(this);
  loadingDialog.setWindowTitle(tr("Loading"));
  loadingDialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
  loadingDialog.setFixedSize(300, 100);
  loadingDialog.setModal(true);
  
  QVBoxLayout *layout = new QVBoxLayout(&loadingDialog);
  
  QLabel *loadingLabel = new QLabel(tr("Loading data..."));
  loadingLabel->setAlignment(Qt::AlignCenter);
  QFont font = loadingLabel->font();
  font.setPointSize(12);
  loadingLabel->setFont(font);
  
  QProgressBar *progressBar = new QProgressBar();
  progressBar->setMinimum(0);
  progressBar->setMaximum(0);
  progressBar->setTextVisible(false);
  
  layout->addWidget(loadingLabel);
  layout->addWidget(progressBar);
  
  // Show dialog and process events to ensure it appears
  loadingDialog.show();
  QCoreApplication::processEvents();

  try {
    model.updateFromFile(path);
      
    if (!model.hasData()) {
      loadingDialog.close();
      QMessageBox::critical(this, tr("Data Error"), tr("The selected file contains no valid data."));
      return;
    }

    model.dataProcessing();

    // Add chart update methods below
    tab1->updateCharts();
    tab3->updateCharts();
    tab4->updateCharts();
    tab5->updateCharts();
    
    switchToTabView();
    loadDataWidget->setDataLoaded(true);

  } catch (const std::exception& error) {
    loadingDialog.close();
    QMessageBox::critical(this, tr("CSV File Error"), error.what());
  }
    
  loadingDialog.close();
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

void WaterWindow::createStatusBar()
{
  fileInfo = new QLabel("No file loaded");
  QStatusBar* status = statusBar();
  status->addWidget(fileInfo);
}

void WaterWindow::about()
{
  QMessageBox::about(this, tr("About Water Quality Monitor"),
    tr("Water Quality Monitor displays water data loaded from "
    "a CSV file produced by the UK Environment Agency.\n\n"
    "(c) 2024 Nick Efford and Group 37"));
}

void WaterWindow::setupStyles()
{
  QString styleSheet = QString(R"(
    QMainWindow {
      background-color: %1;
      color: %2;
    }

    QMenuBar {
      background-color: %3;
      color: %2;
      border: none;
      padding: %4px;
    }

    QMenuBar::item {
      background-color: transparent;
      padding: %4px %5px;
      border-radius: %6px;
      margin-right: %4px;
    }

    QMenuBar::item:selected {
      background-color: %7;
    }

    QMenu {
      background-color: %3;
      color: %2;
      border: 1px solid %8;
      border-radius: %6px;
      padding: %4px;
    }

    QMenu::item {
      padding: %4px 30px %4px 20px;
      border-radius: %6px;
      margin: %9px;
    }

    QMenu::item:selected {
      background-color: %7;
    }

    QStatusBar {
      background-color: %3;
      color: %2;
      border-top: 1px solid %8;
    }

    QStatusBar QLabel {
      color: %2;
      padding: %4px %5px;
    }

    QTabWidget::pane {
      border: 1px solid %8;
      border-radius: %6px;
      background-color: %10;
      top: %11px;
    }

    QTabWidget::tab-bar {
      alignment: center;
      top: %11px;
      left: %11px;
    }

    QTabBar::tab {
      background-color: %10;
      color: %2;
      padding: %5px %12px;
      border: none;
      border-top-left-radius: %6px;
      border-top-right-radius: %6px;
      margin-right: %9px;
      min-width: 100px;
    }

    QTabBar::tab:selected {
      background-color: %13;
      color: white;
    }

    QTabBar::tab:hover:!selected {
      background-color: %7;
    }

    QProgressDialog {
      background-color: %1;
      color: %2;
    }

    QProgressDialog QLabel {
      color: %2;
      font-weight: bold;
    }

    QMessageBox {
      background-color: %1;
      color: %2;
    }

    QMessageBox QLabel {
      color: %2;
    }

    QFileDialog {
      background-color: %1;
      color: %2;
    }
  )")
  .arg(Styles::backgroundColor())              // %1
  .arg(Styles::textColor())                    // %2
  .arg(Styles::menuBackground())               // %3
  .arg(Styles::PADDING_SMALL)                  // %4
  .arg(Styles::PADDING_MEDIUM)                 // %5
  .arg(Styles::BORDER_RADIUS)                  // %6
  .arg(Styles::menuHover())                    // %7
  .arg(Styles::borderColor())                  // %8
  .arg(Styles::SPACING_SMALL)                  // %9
  .arg(Styles::tabBackground())                // %10
  .arg(Styles::PADDING_LARGE)                  // %11
  .arg(Styles::PADDING_LARGE * 2)              // %12
  .arg(Styles::selectedTabBackground());       // %13

  this->setStyleSheet(styleSheet);
}