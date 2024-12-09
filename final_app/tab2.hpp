#pragma once

#include <QtWidgets/QWidget>
#include <QtCharts/QChartView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QLineEdit>
#include <QStandardItemModel>
#include <QtWidgets/QComboBox>
#include "model.hpp"

class QLabel;

class Tab2 : public QWidget {
    Q_OBJECT

public:
    Tab2(WaterModel* model, QWidget* parent = nullptr);

    // Function to update charts (called from window.cpp)
    void updateCharts();

private slots:
    void filterTable(const QString& searchText);
    void onPollutantSelectionChanged(int index);

private:
    void createWidgets();
    void createLayout();
    void setupStyles();
    void loadData(); // Load data from CSV file
    void updateTableAndChart(); // Update table and chart with filtered data

    WaterModel* waterModel;
    QLabel* placeholder;
    QComboBox* pollutantFilter;
    QChartView* chartView;
    QTableView* dataTable;
    QLineEdit* searchBar;
    QStandardItemModel* tableModel;
    QStringList pollutants;
    QList<QStringList> csvData;
};
