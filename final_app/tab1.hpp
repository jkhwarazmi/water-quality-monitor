// Header for the first card - Dashboard
// Jamil

#pragma once

#include <QWidget>
#include <QtCharts>
#include "model.hpp"

class QPushButton;
class QLabel;
class QScrollArea;
class QVBoxLayout;
class QGridLayout;

class Tab1: public QWidget
{
  Q_OBJECT

  public:
    Tab1(WaterModel* model, QWidget* parent = nullptr);

    // This actually sets up the charts w/ the model data
    void updateCharts();

  signals:
    void reloadRequested();

  private:
    WaterModel* waterModel;
    QPushButton *reloadButton;
    QLabel *title;
    
    // For the charts
    QLabel *stats;
    QScrollArea *scrollArea;
    QWidget *chartContainer;
    QGridLayout *chartLayout;

    // The charts themselves
    QChartView *complianceChartView;
    QPieSeries *complianceSeries;

    QChartView *locationChartView;
    QBarSeries *locationSeries;

    QChartView *determinandChartView;
    QBarSeries *determinandSeries;

    QChartView *timelineChartView;
    QLineSeries *timelineSeries;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;

    // This creates empty charts
    void createCharts();

    // Chart update methods
    void updateStats();
    void updateComplianceChart();
    void updateLocationChart();
    void updateDeterminandChart();
    void updateTimelineChart();
};