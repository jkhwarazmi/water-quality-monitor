// Modified COMP2811 Coursework 1 sample solution: Sample class

#include "sample.hpp"


Sample::Sample(
  int easting, 
  int northing, 
  const QString& location,
  const QString& dateTime,
  const QString& pollutant,
  const QString& pollutantDefinition,
  double result,
  const QString& unit,
  const QString& materialType,
  bool isCompliance,
  const QString& purpose
):

  easting(easting),
  northing(northing),
  location(location),
  dateTime(QDateTime::fromString(dateTime, Qt::ISODate)),
  pollutant(pollutant),
  pollutantDefinition(pollutantDefinition),
  result(result),
  unit(unit),
  materialType(materialType),
  isCompliance(isCompliance),
  purpose(purpose)
{
}