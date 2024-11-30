// Modified COMP2811 Coursework 2 solution: data model

#pragma once

#include <QAbstractTableModel>
#include <QMap>
#include <QSet>
#include <QString>
#include <QVector>
#include "dataset.hpp"

class WaterModel: public QAbstractTableModel 
{
  Q_OBJECT

  public:
    WaterModel(QObject* parent = nullptr): QAbstractTableModel(parent) {}
      
    void updateFromFile(const QString&);
    bool hasData() const { return dataset.size() > 0; }

    // Helper method to get pollutant thresholds, needs to be manually filled in
    // For Red, Amber, Green colour system
    double getPollutantThreshold(const QString&) const;
      
    int rowCount(const QModelIndex& index) const { return dataset.size(); }
    int columnCount(const QModelIndex& index) const { return 11; }
    
    QVariant data(const QModelIndex&, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;

    // NEW - Added getter methods for data structures
    QMap<QString, QVector<double>> getPollutantData() const { return pollutantData; }
    QMap<QString, QString> getPollutantUnits() const { return pollutantUnits; }
    QMap<QString, QVector<QString>> getSamplingPointData() const { return samplingPointData; }
    QMap<QString, QString> getPollutantCategories() const { return pollutantCategories; }
    QMap<QString, int> getMaterialTypeCounts() const { return materialTypeCounts; }
    QMap<QString, QVector<double>> getPollutantsByLocation() const { return pollutantsByLocation; }
    QMap<QString, int> getComplianceCounts() const { return complianceCounts; }
    QMap<QString, int> getPurposeCounts() const { return purposeCounts; }
    QList<QPair<QString, double>>getTopPollutants() const { return topPollutants; }
    QMap<QString, int> getCategoryCounts() const { return categoryCounts; }

    // NEW - Added getter methods for summary statistics
    int getUniquePollutantCount() const { return pollutantData.size(); }
    int getUniqueLocationCount() const { return samplingPointData.size(); }

  private:
    WaterDataset dataset;

    // NEW - Added helper hashmaps for data processing, O(1) lookup
    // If you need to add more data structures, do so here
    // Remember to make a public getter method for each one
    QSet<QString> uniquePollutants;
    QMap<QString, QVector<double>> pollutantData;
    QMap<QString, QString> pollutantUnits;
    QMap<QString, QVector<QString>> samplingPointData;
    QMap<QString, QString> pollutantCategories;
    QMap<QString, int> materialTypeCounts;
    QMap<QString, QVector<double>> pollutantsByLocation;
    QMap<QString, int> complianceCounts;
    QMap<QString, int> purposeCounts;
    QList<QPair<QString, double>> topPollutants;
    QMap<QString, int> categoryCounts;

    // NEW - Added method to populate these data structures, modify if you add more
    void processData();
    
    // NEW - I.e. you want to define fluorinated or chlorinated compounds, metals etc.
    void categoriseData(const QString& pollutant, const QString& definition);
};