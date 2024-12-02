// Modified COMP2811 Coursework 1 sample solution: Sample class

#pragma once

#include <QString>
#include <QDateTime>

class Sample 
{
  public:
    Sample(
      int easting,
      int northing,
      const QString &location,
      const QString &dateTime,
      const QString &pollutant,
      const QString &pollutantDefinition,
      double result,
      const QString &unit,
      const QString &materialType,
      bool isCompliance,
      const QString &purpose
    );

    int getEasting() const { return easting; }
    int getNorthing() const { return northing; }
    QString getLocation() const { return location; }
    QDateTime getDateTime() const { return dateTime; }
    QString getPollutant() const { return pollutant; }
    QString getPollutantDefinition() const { return pollutantDefinition; }
    double getResult() const { return result; }
    QString getUnit() const { return unit; }
    QString getMaterialType() const { return materialType; }
    bool isComplianceSample() const { return isCompliance; }
    QString getPurpose() const { return purpose; }

  private:
    int easting;                  // sample.samplingPoint.easting
    int northing;                 // sample.samplingPoint.northing
    QString location;             // sample.samplingPoint.label
    QDateTime dateTime;           // sample.sampleDateTime
    QString pollutant;            // determinand.label
    QString pollutantDefinition;  // determinand.definition
    double result;                // result
    QString unit;                 // determinand.unit.label
    QString materialType;         // sample.sampledMaterialType.label
    bool isCompliance;            // sample.isComplianceSample
    QString purpose;              // sample.purpose.label
};