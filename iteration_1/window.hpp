// Modified COMP2811 Coursework 2 sample solution: main window

#pragma once

#include <QMainWindow>
#include <QTranslator>
#include "model.hpp"

class QString;
class QLabel;
class QPushButton;
class QTableView;
class QStackedWidget;
class QTabWidget;
class Tab1;
class Tab2;
class Tab3;
class LoadDataWidget;
class QTranslator;

class WaterWindow: public QMainWindow
{
  Q_OBJECT

  public:
    WaterWindow();

  private:
    QStackedWidget *stackedWidget;
    LoadDataWidget *loadDataWidget;
    QTabWidget *tabWidget;
      
    Tab1 *tab1;
    Tab2 *tab2;
    Tab3 *tab3;

    WaterModel model;
    QString dataLocation;

    void createMainWidget();
    void createMenus();
    void switchToTabView();
    void loadData(const QString& path);

  private slots:
    void setDataLocation();
    void setPath();
    void switchToLoadView();
    void about();
};