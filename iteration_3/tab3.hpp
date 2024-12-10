// Header for third card - Environmental Litter Indicators

#pragma once

#include <QWidget>
#include <QtCharts>
#include "model.hpp"

class QPushButton;
class QLabel;
class QScrollArea;
class QVBoxLayout;
class QGridLayout;

class Tab3: public QWidget
{
  Q_OBJECT

  public:
    Tab3(WaterModel *model, QWidget *parent = nullptr);
    //new
    void updateCharts();

  private:
    WaterModel* waterModel;
    QLabel *placeholder;
    
    // declares a private member variable *litterChartView* which is a pointer to a *QChartView* object
    QChartView *litterChartView;
    // declares a private member variable *litterSeries* which is a pointer to a *QBarSeries* object
    QBarSeries *litterSeries;
    
    QChartView *waterTypeChartView; 
    QChartView *waterTypeChartView2; 
    QPieSeries *waterTypeSeries_inner; 
    QPieSeries *waterTypeSeries_outer; 

    QScrollArea *scrollArea; 
    QScrollArea *textScrollArea; 
    QWidget *graphContainer; 
    QGridLayout *graphLayout; 

    QTextEdit *stats; 
    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;

    void createCharts();
};
