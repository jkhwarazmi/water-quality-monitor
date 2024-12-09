#pragma once

#include <QtWidgets/QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QLineEdit>
#include <QStandardItemModel>
#include "model.hpp"
#include <QLabel>

class Tab2 : public QWidget {
    Q_OBJECT

public:
    explicit Tab2(WaterModel* model, QWidget* parent = nullptr);

    // Function to update charts (called from window.cpp)
    void updateCharts();

private slots:
    void filterTable(const QString& searchText);
    void onPollutantSelectionChanged(int index);

private:
    void createWidgets();
    void createLayout();
    void setupStyles();
    void updateTableAndChart();  // Update table and chart based on selected pollutant
    void loadData();  // Load data from CSV file into model (if required)

    WaterModel* waterModel;  // Pointer to model to fetch pollutant data
    QLabel* title;           // Title label
    QComboBox* pollutantFilter; // Dropdown filter for pollutants
    QComboBox* timeRangeFilter;  // Dropdown for time ranges (Last week, month, etc.)
    QChartView* chartView;     // Chart view for graphical representation
    QTableView* dataTable;     // Table view for tabular data
    QLineEdit* searchBar;     // Search bar for filtering table data
    QStandardItemModel* tableModel;  // Model for the table
    QStringList pollutants;   // List to hold pollutants (loaded dynamically)
    QList<QStringList> csvData; // CSV data for table population (if required)
};
