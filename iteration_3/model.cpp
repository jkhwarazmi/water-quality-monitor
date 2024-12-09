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
  Determinand,
  DeterminandDefinition,
  Concentration,
  Unit,
  SampleLocation,
  ComplianceSample,
  SamplePurpose
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
        case Determinand: 
          return sample.getDeterminand();
        case DeterminandDefinition: 
          return sample.getDeterminandDefinition();
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
        double threshold = getDeterminandThreshold(sample.getDeterminand());
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
            .arg(getDeterminandThreshold(sample.getDeterminand()))
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
    case Determinand: return tr("Determinand");
    case DeterminandDefinition: return tr("Determinand Definition");
    case Concentration: return tr("Concentration");
    case Unit: return tr("Unit");
    case SampleLocation: return tr("Sample Location");
    case ComplianceSample: return tr("Compliance Sample");
    case SamplePurpose: return tr("Sample Purpose");
    default: return QVariant();
  }
}

// Helper method to get threshold for a Determinand
double WaterModel::getDeterminandThreshold(const QString& Determinand) const {
  // Example thresholds - replace with actual values
  static const QMap<QString, double> thresholds = {
    {"Endrin", 0.0002},
    {"PhenoxytcAcd", 0.02},
    {"MCPB", 0.01},
    {"Mn- Filtered", 15.0}
  };

  return thresholds.value(Determinand, 0.0);
}

void WaterModel:: dataProcessing() {
  QSet<QString> uniqueLocations;
  QSet<QString> uniqueDeterminands;

  complianceSamples = qMakePair(0, 0);

  QMap<QString, int> tmpSampleLocations;
  sampleLocations.clear();

  QMap<QString, int> tmpDeterminandCounts;
  determinandCounts.clear();

  sampleDates.clear();
  litterLocation.clear(); //@MF
  litterWaterType.clear(); //@MF
  
  // Iterate through the data and calculate summary statistics
  for(int i = 0; i < dataset.size(); i++) {
    uniqueLocations.insert(dataset[i].getLocation());
    uniqueDeterminands.insert(dataset[i].getDeterminandDefinition());

    // should contain the 20 items with litter testing @MF
    if (dataset[i].getResult()!=0 && (dataset[i].getDeterminandDefinition() == "Tarry residues"  || dataset[i].getDeterminandDefinition() == "Bathing Water Profile : Other Litter (incl. plastics)" || dataset[i].getDeterminandDefinition() == "Sewage debris")) { // add the rest later and dataset[i] != 0
      // if statement for the litterLocation aka bar chart
      if (!litterLocation.contains(dataset[i].getLocation())) {
        litterLocation[dataset[i].getLocation()] = new QMap<QString, int>();
        litterLocation[dataset[i].getLocation()]->insert(dataset[i].getDeterminandDefinition(), (dataset[i].getResult()));
      } else if (!litterLocation[dataset[i].getLocation()]->contains(dataset[i].getDeterminandDefinition())) {
        litterLocation[dataset[i].getLocation()]->insert(dataset[i].getDeterminandDefinition(), (dataset[i].getResult()));
      } else {
        litterLocation[dataset[i].getLocation()]->insert(dataset[i].getDeterminandDefinition(), litterLocation[dataset[i].getLocation()]->value(dataset[i].getDeterminandDefinition())+(dataset[i].getResult()));      }
      // if statement for the litterWaterType aka pie chart NEW
      if (!litterWaterType.contains(dataset[i].getMaterialType())) {
        litterWaterType[dataset[i].getMaterialType()] = new QMap<QString, int>();
        litterWaterType[dataset[i].getMaterialType()]->insert(dataset[i].getDeterminandDefinition(), (dataset[i].getResult()));
      } else if (!litterWaterType[dataset[i].getMaterialType()]->contains(dataset[i].getDeterminandDefinition())) {
        litterWaterType[dataset[i].getMaterialType()]->insert(dataset[i].getDeterminandDefinition(), (dataset[i].getResult()));
      } else {
        litterWaterType[dataset[i].getMaterialType()]->insert(dataset[i].getDeterminandDefinition(), litterWaterType[dataset[i].getMaterialType()]->value(dataset[i].getDeterminandDefinition())+(dataset[i].getResult()));      }
    }

    if (dataset[i].isComplianceSample()) {
      complianceSamples.first++;
    } else {
      complianceSamples.second++;
    }

    tmpSampleLocations[dataset[i].getLocation()]++;
    tmpDeterminandCounts[dataset[i].getDeterminandDefinition()]++;
    sampleDates[dataset[i].getDateTime().date()]++;
  }

  QVector<QPair<QString, int>> sortedLocations;
  for(auto i = tmpSampleLocations.constBegin(); i != tmpSampleLocations.constEnd(); i++) {
    sortedLocations.append(qMakePair(i.key(), i.value()));
  }

  // Sort by count in descending order
  std::sort(sortedLocations.begin(), sortedLocations.end(),
    [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
      return a.second > b.second;
  });

  // Take only top 5 for sampleLocations
  int n = qMin(5, sortedLocations.size());
  for(int i = 0; i < n; ++i) {
    sampleLocations[sortedLocations[i].first] = sortedLocations[i].second;
  }

  QVector<QPair<QString, int>> sortedDeterminands;
  for(auto i = tmpDeterminandCounts.constBegin(); i != tmpDeterminandCounts.constEnd(); i++) {
    sortedDeterminands.append(qMakePair(i.key(), i.value()));
  }

  // Sort by count in descending order
  std::sort(sortedDeterminands.begin(), sortedDeterminands.end(),
    [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
      return a.second > b.second;
  });

  // Take only top 5 for determinandCounts
  n = qMin(5, sortedDeterminands.size());
  for(int i = 0; i < n; ++i) {
    determinandCounts[sortedDeterminands[i].first] = sortedDeterminands[i].second;
  }
  
  locationCount = uniqueLocations.size();
  determinandCount = uniqueDeterminands.size();
}