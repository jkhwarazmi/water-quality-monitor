// Modified COMP2811 Coursework 2 solution: data model

#include <QColor>
#include <QBrush>
#include <QDebug>
#include "model.hpp"

enum Column {
  Easting = 0,
  Northing,
  Location,
  DateTime,
  Pollutant,
  PollutantDefinition,
  Concentration,
  Unit,
  SampleLocation,
  ComplianceSample,
  SamplePurpose
};

WaterModel::~WaterModel() {
// Clear the model data
  beginResetModel();
  endResetModel();
  
  // Clear the database for faster loading next time
  QSqlDatabase db = QSqlDatabase::database();
  if (db.isValid() && db.isOpen()) {
    QSqlQuery query(db);
    if (!query.exec("DELETE FROM WaterQuality;")) {
      qDebug() << "Failed to clear database:" << query.lastError().text();
    } else {
      qDebug() << "App closed successfully";
    }
  }
}

void WaterModel::updateFromFile(const QString& filename) {
  beginResetModel();
  dataset.loadData(filename.toStdString());
  endResetModel();
}

QVariant WaterModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }

  QSqlQuery query;
  query.prepare("SELECT "
    "[sample.samplingPoint.easting], "
    "[sample.samplingPoint.northing], "
    "[sample.samplingPoint.label], "
    "[sample.sampleDateTime], "
    "[determinand.label], "
    "[determinand.definition], "
    "result, "
    "[determinand.unit.label], "
    "[sample.sampledMaterialType.label], "
    "[sample.isComplianceSample], "
    "[sample.purpose.label] "
    "FROM WaterQuality LIMIT 1 OFFSET ?"
  );
  
  query.bindValue(0, index.row());
  
  if (!query.exec() || !query.next()) {
    return QVariant();
  }

  switch (role) {
    case Qt::DisplayRole:
      switch (index.column()) {
        case Easting: return query.value(0);
        case Northing: return query.value(1);
        case Location: return query.value(2);
        case DateTime: return query.value(3);
        case Pollutant: return query.value(4);
        case PollutantDefinition: return query.value(5);
        case Concentration: return query.value(6);
        case Unit: return query.value(7);
        case SampleLocation: return query.value(8);
        case ComplianceSample: return query.value(9).toBool() ? tr("Yes") : tr("No");
        case SamplePurpose: return query.value(10);
      }
      break;

    case Qt::TextAlignmentRole:
      switch (index.column()) {
        case Easting:
        case Northing:
        case Concentration:
          return int(Qt::AlignRight | Qt::AlignVCenter);
        default:
          return int(Qt::AlignLeft | Qt::AlignVCenter);
      }

    case Qt::BackgroundRole:
      if (index.column() == Concentration) {
        double value = query.value(6).toDouble();
        QString pollutant = query.value(4).toString();
        double threshold = getPollutantThreshold(pollutant);
        
        // Colour cells based on pollutant concentration
        if (value > threshold) {
          return QBrush(QColor(204, 31, 40));  // Red
        } else if (value > threshold * 0.8) {
          return QBrush(QColor(253, 183, 0));  // Amber
        } else {
          return QBrush(QColor(32, 125, 32));  // Green
        }
      }
      break;

    case Qt::ToolTipRole:
      if (index.column() == Concentration) {
        return tr("Result: %1 %2\nThreshold: %3 %4")
          .arg(query.value(6).toDouble())
          .arg(query.value(7).toString())
          .arg(getPollutantThreshold(query.value(4).toString()))
          .arg(query.value(7).toString());
      }
      break;
  }
  return QVariant();
}

double WaterModel::getPollutantThreshold(const QString& pollutant) const {
  return pollutantThresholds.value(pollutant, 0.0);
}

QVariant WaterModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (orientation == Qt::Vertical) {
    return section + 1;
  }

  switch (section) {
    case Easting: return tr("Easting");
    case Northing: return tr("Northing");
    case Location: return tr("Location Name");
    case DateTime: return tr("Date/Time");
    case Pollutant: return tr("Pollutant");
    case PollutantDefinition: return tr("Pollutant Definition");
    case Concentration: return tr("Concentration");
    case Unit: return tr("Unit");
    case SampleLocation: return tr("Sample Location");
    case ComplianceSample: return tr("Compliance Sample");
    case SamplePurpose: return tr("Sample Purpose");
    default: return QVariant();
  }
}

QMap<QString, int> WaterModel::getComplianceStats() const {
  QMap<QString, int> stats;
  QSqlQuery query(database);
  
  query.exec("SELECT COUNT(*) FROM WaterQuality WHERE [sample.isComplianceSample] = 1");
  if (query.next()) {
    stats["Compliance"] = query.value(0).toInt();
  }
  
  query.exec("SELECT COUNT(*) FROM WaterQuality WHERE [sample.isComplianceSample] = 0");
  if (query.next()) {
    stats["Non-Compliance"] = query.value(0).toInt();
  }
  
  return stats;
}