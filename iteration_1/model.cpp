// Modified COMP2811 Coursework 2 solution: data model

#include "model.hpp"
#include <QColor>
#include <QFont>
#include <QBrush>

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
  SamplePurpose,
  ColumnCount
};


void WaterModel::updateFromFile(const QString& filename) {
  beginResetModel();
  dataset.loadData(filename.toStdString());
  endResetModel();
}

QVariant WaterModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid() || index.row() >= dataset.size()) {
    return QVariant();
  }

  const Sample& sample = dataset[index.row()];

  switch (role) {
    case Qt::DisplayRole:
      switch (index.column()) {
        case Easting: 
          return sample.getEasting();
        case Northing: 
          return sample.getNorthing();
        case Location: 
          return sample.getLocation();
        case DateTime: 
          return sample.getDateTime();
        case Pollutant: 
          return sample.getPollutant();
        case PollutantDefinition: 
          return sample.getPollutantDefinition();
        case Concentration:
          return sample.getResult();
        case Unit: 
          return sample.getUnit();
        case SampleLocation: 
          return sample.getMaterialType();
        case ComplianceSample: 
          return sample.isComplianceSample() ? tr("Yes") : tr("No");
        case SamplePurpose: 
          return sample.getPurpose();
      }
      break;

    case Qt::TextAlignmentRole:
      // Right-align numbers, left-align text
      switch (index.column()) {
        case Easting:
        case Northing:
        case Concentration:
          return int(Qt::AlignRight | Qt::AlignVCenter);
        default:
          return int(Qt::AlignLeft | Qt::AlignVCenter);
      }

    case Qt::BackgroundRole:
      // Color coding for compliance and concentration
      if (index.column() == Concentration) {
        double value = sample.getResult();
        double threshold = getPollutantThreshold(sample.getPollutant());
        // Amber = 80% of red
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
      // Add tooltips
      switch (index.column()) {
        case Concentration:
          return tr("Result: %1 %2\nThreshold: %3 %4")
            .arg(sample.getResult())
            .arg(sample.getUnit())
            .arg(getPollutantThreshold(sample.getPollutant()))
            .arg(sample.getUnit());
        case ComplianceSample:
          return sample.isComplianceSample() 
            ? tr("This is a compliance sample")
            : tr("This is not a compliance sample");
        default:
          return QVariant();
      }
  }
  return QVariant();
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

// Helper method to get threshold for a pollutant
double WaterModel::getPollutantThreshold(const QString& pollutant) const {
    // Example thresholds - replace with actual values
    static const QMap<QString, double> thresholds = {
        {"Endrin", 0.0002},
        {"PhenoxytcAcd", 0.02},
        {"MCPB", 0.01},
        {"Mn- Filtered", 15.0}
    };

    return thresholds.value(pollutant, 0.0);
}