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
  SamplePurpose
};


void WaterModel::updateFromFile(const QString& filename) {
  beginResetModel();
  dataset.loadData(filename.toStdString());
  processData();
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
    // Example thresholds - complete and replace with actual values
    // Link the source here when complete
    static const QMap<QString, double> thresholds = {
        {"Endrin", 0.0002},
        {"PhenoxytcAcd", 0.02},
        {"MCPB", 0.01},
        {"Mn- Filtered", 15.0}
    };

    return thresholds.value(pollutant, 0.0);
}

// NEW - Added method to populate data structures
void WaterModel::processData() 
{
  // Clear existing data structures
  pollutantData.clear();
  pollutantUnits.clear();
  samplingPointData.clear();
  pollutantCategories.clear();
  materialTypeCounts.clear();
  pollutantsByLocation.clear();
  uniquePollutants.clear();
  complianceCounts.clear();
  purposeCounts.clear();
  topPollutants.clear();
  categoryCounts.clear();

  for (int i = 0; i < rowCount(QModelIndex()); i++) {
    QString pollutant = data(index(i, Pollutant), Qt::DisplayRole).toString();
    QString concentration = data(index(i, Concentration), Qt::DisplayRole).toString();
    QString unit = data(index(i, Unit), Qt::DisplayRole).toString();
    QString samplingPoint = data(index(i, Location), Qt::DisplayRole).toString();
    QString materialType = data(index(i, SampleLocation), Qt::DisplayRole).toString();
    QString definition = data(index(i, PollutantDefinition), Qt::DisplayRole).toString();
    QString isCompliance = data(index(i, ComplianceSample), Qt::DisplayRole).toString();
    QString purpose = data(index(i, SamplePurpose), Qt::DisplayRole).toString();
    
    // Count compliance
    complianceCounts[isCompliance]++;

    // Count purposes
    purposeCounts[purpose]++;

    // Count material types
    materialTypeCounts[materialType]++;

    // Track unique pollutants
    uniquePollutants.insert(pollutant);

    // Skip non-numeric values but track them
    if (!concentration.startsWith("<")) {
      double value = concentration.toDouble();
      pollutantData[pollutant].append(value);
      pollutantUnits[pollutant] = unit;
      pollutantsByLocation[samplingPoint].append(value);

      if (!samplingPointData.contains(samplingPoint)) {
        samplingPointData[samplingPoint] = QVector<QString>();
      }
      samplingPointData[samplingPoint].append(pollutant + ":" + QString::number(value));
    }

    // Calculate averages
    QMap<QString, double> averages;
    for (auto it = pollutantData.begin(); it != pollutantData.end(); it++) {
      if (!it.value().isEmpty()) {
        double sum = 0;
        for (double value : it.value()) {
          sum += value;
        }
        averages[it.key()] = sum / it.value().size();
      }
    }

    // Sort and store top pollutants
    for (auto it = averages.begin(); it != averages.end(); ++it) {
      topPollutants.append(qMakePair(it.key(), it.value()));
    }
    
    std::sort(topPollutants.begin(), topPollutants.end(),
      [](const QPair<QString, double>& a, const QPair<QString, double>& b) {
        return a.second > b.second;
      }
    );

    // Enhanced pollutant categorisation
    categoriseData(pollutant, definition);

    // Count categories
    categoryCounts[pollutantCategories[pollutant]]++;
  }
}

// NEW - Added method to categorise pollutants
// This isn't very specific, you may have to look into the data more to see how
// They actually categorise your specific pollutants
void WaterModel::categoriseData(const QString& pollutant, const QString& definition) 
{
  if (definition.contains("Nitrogen", Qt::CaseInsensitive) || 
    definition.contains("Phosphate", Qt::CaseInsensitive)) {
    pollutantCategories[pollutant] = "Nutrients";
  } else if (definition.contains("Carbon", Qt::CaseInsensitive) || 
    definition.contains("Organic", Qt::CaseInsensitive)) {
    pollutantCategories[pollutant] = "Organic Compounds";
  } else if (definition.contains("chlor", Qt::CaseInsensitive)) {
    pollutantCategories[pollutant] = "Chlorinated Compounds";
  } else if (definition.contains("fluor", Qt::CaseInsensitive)) {
    pollutantCategories[pollutant] = "Fluorinated Compounds";
  } else {
    pollutantCategories[pollutant] = "Other";
  }
}