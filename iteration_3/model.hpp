// Modified COMP2811 Coursework 2 solution: data model

#pragma once

#include <QAbstractTableModel>
#include <QMap>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include "dataset.hpp"

class WaterModel: public QAbstractTableModel 
{
    Q_OBJECT

public:
    WaterModel(QObject* parent = nullptr): QAbstractTableModel(parent) { database = QSqlDatabase::database(); }
    ~WaterModel();
    
    void updateFromFile(const QString &);
    bool hasData() const { return dataset.size() > 0; };

    int rowCount(const QModelIndex &index) const { return dataset.size(); };
    int columnCount(const QModelIndex& index) const { return 11; }
    QVariant data(const QModelIndex&, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;

    // Helper method for colouring cells
    double getPollutantThreshold(const QString&) const;

    // Helper methods for charts
    QMap<QString, int> getComplianceStats() const;

private:
    WaterDataset dataset;
    QSqlDatabase database;
    
    // Example pollutant thresholds, modify as needed
    // Include a source for the threshold values (e.g. UK Environment Agency)
    const QMap<QString, double> pollutantThresholds {
      {"Endrin", 0.0002},
      {"PhenoxytcAcd", 0.02},
      {"MCPB", 0.01},
      {"Mn- Filtered", 15.0}
    };
};