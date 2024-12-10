#pragma once

#include <QtWidgets/QWidget>
#include <QtCharts/QChartView>
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
    void updateCharts();

private slots:
    void filterTable(const QString& searchText);
    void onPollutantSelectionChanged(int index);

private:
    void createWidgets();
    void createLayout();
    void setupStyles();
    void populateDropdowns();
    void updateTable(const QVector<QPair<QDateTime, double>>& dataPoints);

    WaterModel* waterModel;
    QLabel* title;
    QComboBox* pollutantFilter;
    QComboBox* timeRangeFilter;
    QChartView* chartView;
    QTableView* dataTable;
    QLineEdit* searchBar;
    QStandardItemModel* tableModel;
};
