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

void WaterWindow::about()
{
  QMessageBox::about(this, tr("About Water Quality Monitor"),
    tr("Water Quality Monitor displays water data loaded from "
    "a CSV file produced by the UK Environment Agency.\n\n"
    "(c) 2024 Nick Efford and Group 37"));
}