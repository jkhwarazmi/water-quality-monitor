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
    void updateCharts();  // Keep this public if it's called externally (in window.cpp)

private slots:
    void filterTable(const QString& searchText); // For search bar
    void onPollutantSelectionChanged(int index);  // Slot for pollutant selection change

private:
    // Private methods
    void createWidgets();
    void createLayout();
    void setupStyles();
    void loadData();  // Method to load data from CSV file
    void updateTableAndChart(); // Method to update table and chart with data

    // Private member variables
    WaterModel* waterModel;
    QLabel* placeholder;
    QComboBox* pollutantFilter;
    QChartView* chartView;
    QTableView* dataTable;
    QLineEdit* searchBar;
    QStandardItemModel* tableModel;
    QStringList pollutants;  // Store list of pollutants loaded from CSV
    QList<QStringList> csvData;  // Store all rows from the CSV data
};
