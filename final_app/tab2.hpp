#pragma once

#include <QtWidgets/QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QTableView>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QStandardItemModel>
#include "model.hpp"

class QLabel;

class Tab2 : public QWidget {
    Q_OBJECT

public:
    explicit Tab2(WaterModel* model, QWidget* parent = nullptr);

    // Function to update charts and tables (called from window.cpp)
    void updateCharts();

private slots:
    void filterTable(const QString& searchText); // Search functionality for filtering
    void onPollutantSelectionChanged(int index); // Update charts when dropdown selection changes

private:
    void createWidgets();           // Create UI elements
    void createLayout();            // Arrange UI elements
    void setupStyles();             // Apply styles
    void loadData();                // Load data from data.csv
    void updateTableAndChart();     // Populate table and chart dynamically
    void populateTimeRange();       // Optionally filter by time ranges (if required)

    WaterModel* waterModel;         // Pointer to the WaterModel for accessing data
    QLabel* title;                  // Title label for the page
    QComboBox* pollutantFilter;     // Dropdown filter for pollutants
    QComboBox* timeRangeFilter;     // Dropdown for time range selection
    QChartView* chartView;          // Chart view for displaying trends
    QTableView* dataTable;          // Table view for detailed data
    QLineEdit* searchBar;           // Search bar for filtering pollutants in the table
    QStandardItemModel* tableModel; // Model for managing table data
    QStringList pollutants;         // List of available pollutants
    QList<QStringList> csvData;     // Data loaded from the CSV file for display
};
