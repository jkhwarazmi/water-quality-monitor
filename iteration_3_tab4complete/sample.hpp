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
      const QString &determinand,
      const QString &determinandDefinition,
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
    QString getDeterminand() const { return determinand; }
    QString getDeterminandDefinition() const { return determinandDefinition; }
    double getResult() const { return result; }
    QString getUnit() const { return unit; }
    QString getMaterialType() const { return materialType; }
    bool isComplianceSample() const { return isCompliance; }
    QString getPurpose() const { return purpose; }

  private:
    int easting;                    // sample.samplingPoint.easting
    int northing;                   // sample.samplingPoint.northing
    QString location;               // sample.samplingPoint.label
    QDateTime dateTime;             // sample.sampleDateTime
    QString determinand;            // determinand.label
    QString determinandDefinition;  // determinand.definition
    double result;                  // result
    QString unit;                   // determinand.unit.label
    QString materialType;           // sample.sampledMaterialType.label
    bool isCompliance;              // sample.isComplianceSample
    QString purpose;                // sample.purpose.label
};