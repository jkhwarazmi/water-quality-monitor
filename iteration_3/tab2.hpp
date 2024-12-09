// Header for second card - Pollutant Overview
// Nicolas

#pragma once

#include <QtWidgets/QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QLineEdit> // For search bar
#include <QStandardItemModel>
#include "model.hpp"

class QLabel;

class Tab2 : public QWidget {
    Q_OBJECT

public:
    Tab2(WaterModel *model, QWidget *parent = nullptr);

private slots:
    void populateData();
    void filterTable(const QString& searchText);

private:
    WaterModel *waterModel;
    QLabel *title;
    QComboBox *pollutantFilter;
    QComboBox *timeRangeFilter;
    QChartView *chartView;
    QTableView *dataTable;
    QLineEdit *searchBar; // Search bar
    QStandardItemModel *tableModel;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};