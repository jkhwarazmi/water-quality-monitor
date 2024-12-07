// Header for second card - Pollutant Overview
// Nicolas

#pragma once

#include <QtWidgets/QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableView>
#include "model.hpp"

class QLabel;

class Tab2 : public QWidget {
    Q_OBJECT

public:
    Tab2(WaterModel *model, QWidget *parent = nullptr);

private slots:
    void populateData();

private:
    WaterModel *waterModel;
    QLabel *placeholder;
    QComboBox *pollutantFilter;
    QComboBox *timeRangeFilter;
    QChartView *chartView;
    QTableView *dataTable;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
