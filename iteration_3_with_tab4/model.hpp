// Modified COMP2811 Coursework 2 solution: data model

#pragma once

#include <QAbstractTableModel>
#include <QMap>
#include <QPair>
#include "dataset.hpp"

class WaterModel: public QAbstractTableModel 
{
  Q_OBJECT

  public:
    WaterModel(QObject* parent = nullptr): QAbstractTableModel(parent) {}
      
    void updateFromFile(const QString&);
    bool hasData() const { return dataset.size() > 0; }
    double getDeterminandThreshold(const QString&) const;
      
    int rowCount(const QModelIndex& index) const { return dataset.size(); }
    int columnCount(const QModelIndex& index) const { return 11; }
    
    QVariant data(const QModelIndex&, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;

    // Add additional processing inside here
    void dataProcessing();

    // Getters for chart data
    QMap<QString, QVector<double>> getPollutantData() const { return pollutantData; }
    QMap<QString, QVector<QPair<QString, double>>> getSamplingPointData() const { return samplingPointData; }
    int getLocationCount() const { return locationCount; }
    int getDeterminandCount() const { return determinandCount; }
    QPair<int, int> getComplianceSampleCount() const { return complianceSamples; }
    QMap<QString, int> getSampleLocations() const { return sampleLocations; }
    QMap<QString, int> getDeterminandCounts() const { return determinandCounts; }
    QMap<QDate, int> getSampleDates() const { return sampleDates; }

  private:
    WaterDataset dataset;

    // Variables for chart data
    int locationCount = 0;
    int determinandCount = 0;
    QMap<QString, QVector<double>> pollutantData;
    QMap<QString, QVector<QPair<QString, double>>> samplingPointData; // Corrected type
    QPair<int, int> complianceSamples = qMakePair(0, 0);
    QMap<QString, int> sampleLocations = QMap<QString, int>();
    QMap<QString, int> determinandCounts = QMap<QString, int>();
    QMap<QDate, int> sampleDates = QMap<QDate, int>();
};