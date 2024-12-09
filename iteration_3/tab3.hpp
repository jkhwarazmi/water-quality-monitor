// Header for third card - Environmental Litter Indicators
// Mila

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
    //NEW
    // declares a private member variable *litterChartView* which is a pointer to a *QChartView* object
    QChartView *litterChartView;
    // declares a private member variable *litterSeries* which is a pointer to a *QBarSeries* object
    QBarSeries *litterSeries;
    
    // NEW NEW
    QChartView *waterTypeChartView; // was complianceChartView
    QPieSeries *waterTypeSeries; // was complianceSeries

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
    //new
    void createCharts();
};
