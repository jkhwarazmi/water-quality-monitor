// Header for the first card - Dashboard
// Jamil

#pragma once

#include <QWidget>
#include "model.hpp"
#include <QtCharts>

class QPushButton;
class QLabel;

class Tab1: public QWidget
{
  Q_OBJECT

  public:
    Tab1(WaterModel* model, QWidget* parent = nullptr);

    // Charts
    void updateComplianceChart();

  signals:
    void reloadRequested();

  private:
    WaterModel* waterModel;
    QPushButton *reloadButton;
    QLabel *placeholder;
    QChartView *chartView;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};