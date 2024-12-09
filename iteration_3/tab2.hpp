// Header for second card - Pollutant Overview
// Nicolas

#pragma once

#include <QtWidgets/QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QLineEdit> // For search bar
#include <QtWidgets/QPushButton> // For the "View Raw Data" button
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
    void showRawDataDialog(); // Slot to handle the "View Raw Data" button click

private:
    WaterModel *waterModel;
    QLabel *placeholder;
    QComboBox *pollutantFilter;
    QComboBox *timeRangeFilter;
    QChartView *chartView;
    QTableView *dataTable;
    QLineEdit *searchBar; // Search bar
    QPushButton *viewRawDataButton; // Button for viewing raw data
    QStandardItemModel *tableModel;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
