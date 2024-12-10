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
class Tab4;
class Tab5;
class Tab6;
class Tab7;
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
    QLabel *fileInfo;
      
    Tab1 *tab1;
    Tab2 *tab2;
    Tab3 *tab3;
    Tab4 *tab4;
    Tab5 *tab5;
    Tab6 *tab6;
    Tab7 *tab7;

    WaterModel model;
    QString dataLocation;
    QTranslator translator;

    void createMainWidget();
    void createMenus();
    void createLanguageMenu();
    void createStatusBar();
    void switchToTabView();
    void loadData(const QString& path);
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;

  private slots:
    void setDataLocation();
    void setPath();
    void switchToLoadView();
    void cancelLoad();
    void about();
    void changeLanguage(const QString &);
    void handleLanguageAction(QAction *);
};
