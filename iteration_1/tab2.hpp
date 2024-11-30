#pragma once

#include <QWidget>
#include <QComboBox>
#include <QChartView>
#include <QTableView>
#include "model.hpp"

QT_CHARTS_USE_NAMESPACE

class QLabel;

class Tab2: public QWidget
{
  Q_OBJECT

  public:
    Tab2(WaterModel *model, QWidget *parent = nullptr);

  private:
    WaterModel* waterModel;

    // UI Elements
    QLabel *placeholder; // Placeholder label (can be removed if not needed anymore)
    QComboBox *pollutantFilter;
    QComboBox *timeRangeFilter;
    QChartView *chartView;
    QTableView *dataTable;

    // Private methods
    void createWidgets();
    void createLayout();
    void setupStyles();
    void populateData();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
