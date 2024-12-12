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
    QString getDeterminandDefinition(const QString& pollutant) const;

    QVector<QPair<QString, double>> getTopLocationsByPollutant(const QString& pollutant) const;

    // Add additional processing inside here
    void dataProcessing();

    // Getters for chart data
    QMap<QString, QVector<QPair<QDateTime, double>>> getPollutantData() const { return pollutantData; }
    QMap<QString, QVector<QPair<QString, double>>> getSamplingPointData() const { return samplingPointData; }
    int getLocationCount() const { return locationCount; }
    int getDeterminandCount() const { return determinandCount; }
    QPair<int, int> getComplianceSampleCount() const { return complianceSamples; }
    QMap<QString, int> getSampleLocations() const { return sampleLocations; }
    QMap<QString, int> getDeterminandCounts() const { return determinandCounts; }
    QMap<QDate, int> getSampleDates() const { return sampleDates; }

    QMap<QString, QVector<double>> getFluorPollutantData() const { return fluorPollutantData; }
    QMap<QString, QVector<QPair<QString, double>>> getFluorLocations() const { return fluorLocations; }
    const QMap<QString, QSet<QString>>& getPollutantToLocations() const { return pollutantToLocations; }
  
    QMap<QString, QMap<QString, int>*> getLitterLocation() const { return litterLocation;}
    QMap<QString, QMap<QString, int>*> getLitterWaterType() const { return litterWaterType;}

    QVector<QPair<QDateTime, double>> getDataForSpecificPollutants(const QString& pollutant) const;
    QMap<QString, QVector<QPair<QDateTime, double>>> getYourPollutantData() const { return pollutantData; }
    QMap<QString, QSet<QString>> getYourPollutantLocations() const { return pollutantLocations; }
  private:
    WaterDataset dataset;

    // Variables for chart data
    int locationCount = 0;
    int determinandCount = 0;

    QMap<QString, QVector<QPair<QString, double>>> samplingPointData; 

    QMap<QString, QVector<double>> fluorPollutantData;              // Stores pollutants with "fluor"
    QMap<QString, QVector<QPair<QString, double>>> fluorLocations; // Stores locations for "fluor" pollutants
    QMap<QString, QSet<QString>> pollutantToLocations; // Maps pollutant to associated locations

    QPair<int, int> complianceSamples = qMakePair(0, 0);
    QMap<QString, int> sampleLocations = QMap<QString, int>();
    QMap<QString, int> determinandCounts = QMap<QString, int>();
    QMap<QDate, int> sampleDates = QMap<QDate, int>();

    QMap<QString, QMap<QString, int>*> litterLocation = QMap<QString, QMap<QString, int>*>();
    QMap<QString, QMap<QString, int>*> litterWaterType = QMap<QString, QMap<QString, int>*>();

    QMap<QString, QVector<QPair<QDateTime, double>>> pollutantData;
    QMap<QString, QSet<QString>> pollutantLocations;
};