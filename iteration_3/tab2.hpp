// Header for second card - Pollutant Overview
// Nicolas

#pragma once

#include <QtWidgets/QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QLineEdit> // For search bar
#include <QtWidgets/QPushButton> // For View Raw Data button
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
    void showRawDataDialog(); // Slot for showing raw data dialog

private:
    WaterModel *waterModel;
    QLabel *placeholder;
    QComboBox *pollutantFilter;
    QComboBox *timeRangeFilter;
    QChartView *chartView;
    QTableView *dataTable;
    QLineEdit *searchBar; // Search bar
    QStandardItemModel *tableModel;
    QPushButton *viewRawDataButton; // Button for viewing raw data

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
