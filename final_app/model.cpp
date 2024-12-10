// Modified COMP2811 Coursework 2 solution: data model

#include "model.hpp"
#include <QColor>
#include <QFont>
#include <QBrush>
#include <QMap>
#include <QVector>
#include <algorithm>

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
  qDebug() << "File loaded: " << filename;

  // Process the dataset for pollutants and time series
  dataProcessing();
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

// determinand.label
QString WaterModel::getDeterminandDefinition(const QString& pollutant) const {
    for (int i = 0; i < rowCount(QModelIndex()); ++i) {
        QString samplePollutant = data(index(i, 4), Qt::DisplayRole).toString(); // Column 4 for pollutant
        if (samplePollutant == pollutant) {
            return data(index(i, 5), Qt::DisplayRole).toString(); // Column 5 for definition
        }
    }
    return QString(); // Return an empty string if no definition is found
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
    // Sarah
    {"Ammonia(N)", 0.2},
    {"BOD ATU", 5.0},
    {"Sld Sus@105C", 20.0},
    {"O Diss %sat", 5.0},
    {"Orthophospht", 0.1},
    {"N Oxidised", 1.0},
    {"Nitrite-N", 0.1},
    {"Nitrate-N", 5.0},
    {"NH3 un-ion", 0.2},
    {"COD as O2", 20.0},
    {"Phosphorus-P", 0.1},
    {"Nitrogen - N", 5.0},
    // Yusuf
    {"Endrin", 0.0002},
    {"PhenoxytcAcd", 0.02},
    {"MCPB", 0.01},
    {"Mn- Filtered", 15.0},
    {"11Cl-PF3OUdS", 0.1},
    {"3:3 FTCA", 0.05},
    {"4:2 FTSA", 0.07},
    {"5:3 FTCA", 0.06},
    {"6:2 FTSA", 0.08},
    {"7:3 FTCA", 0.09},
    {"8:2 FTSA", 0.1},
    {"9Cl-PF3ONS", 0.1},
    {"ADONA", 0.1},
    {"B-[b]-fluora", 0.1},
    {"B-[k]-fluora", 0.1},
    {"EtFOSA", 0.05},
    {"EtFOSAA-B", 0.07},
    {"EtFOSAA-L", 0.07},
    {"FBSA", 0.08},
    {"FLUORENE", 0.2},
    {"FOSA", 0.07},
    {"Fluoranthene", 0.1},
    {"Fluoride - F", 1.5},  // WHO guideline value
    {"Fluoroxypyr", 0.5},
    {"HFPO-DA", 0.07},      // Similar to PFOA/PFOS
    {"MeFOSAA-B", 0.07},
    {"MeFOSAA-L", 0.07},
    {"N-MeFOSA", 0.07},
    {"NFDHA", 0.05},
    {"PFBA", 0.1},
    {"PFBS", 0.1},
    {"PFDS", 0.1},
    {"PFDoS", 0.1},
    {"PFEESA", 0.1},
    {"PFHpS", 0.1},
    {"PFHxS-B", 0.1},
    {"PFHxS-L", 0.1},
    {"PFHxSA", 0.1},
    {"PFMOBA", 0.1},
    {"PFMOPrA", 0.1},
    {"PFNS", 0.1},
    {"PFODA", 0.1},
    {"PFOS", 0.005},
    {"PFOS (B)", 0.1},
    {"PFOS (L)", 0.1},
    {"PFPeS", 0.1},
    {"PFTrDA", 0.1},
    {"PFUnDS", 0.1},
    {"PFdecncAcid", 0.1},
    {"PFdodencAcid", 0.1},
    {"PFecHS", 0.1},
    {"PFheptncAcid", 0.1},
    {"PFhexncAcid", 0.07},
    {"PFnonncAcid", 0.07},
    {"PFoctncAcid", 0.07},
    {"PFpentncAcid", 0.07},
    {"PFtetdncAcid", 0.07},
    {"PFundencAcid", 0.07},
    {"TrCFMethan", 0.07},
    {"pFoctanoate", 0.07}
  };

  return thresholds.value(Determinand, 0.005);
}

void WaterModel::dataProcessing() {
    QSet<QString> uniqueLocations;
    QSet<QString> uniqueDeterminands;

    complianceSamples = qMakePair(0, 0);

    pollutantData.clear();
    samplingPointData.clear();

    fluorPollutantData.clear();
    fluorLocations.clear();

    pollutantToLocations.clear(); // Clear previous mappings

    QMap<QString, int> tmpSampleLocations;
    sampleLocations.clear();

    QMap<QString, int> tmpDeterminandCounts;
    determinandCounts.clear();

    sampleDates.clear();
    litterLocation.clear(); //@MF
    litterWaterType.clear(); //@MF

    QMap<QString, QVector<QPair<QDateTime, double>>> pollutantTimeSeries;

    // Iterate through the data and calculate summary statistics
    for (int i = 0; i < dataset.size(); i++) {

        bool isFluor = dataset[i].getDeterminandDefinition().contains("fluor", Qt::CaseInsensitive) ||
                           dataset[i].getDeterminand().contains("fluor", Qt::CaseInsensitive);

        uniqueLocations.insert(dataset[i].getLocation());
        uniqueDeterminands.insert(dataset[i].getDeterminandDefinition());

        pollutantToLocations[dataset[i].getDeterminand()].insert(dataset[i].getLocation()); // OR SWITCH TO DEFINITION??

        // Process litter data for charts (bar and pie charts) @MF
        if (dataset[i].getResult() != 0 && (dataset[i].getDeterminandDefinition() == "Tarry residues" ||
                                            dataset[i].getDeterminandDefinition() == "Bathing Water Profile : Other Litter (incl. plastics)" ||
                                            dataset[i].getDeterminandDefinition() == "Sewage debris")) {
            // Bar chart data
            if (!litterLocation.contains(dataset[i].getLocation())) {
                litterLocation[dataset[i].getLocation()] = new QMap<QString, int>();
                litterLocation[dataset[i].getLocation()]->insert(dataset[i].getDeterminandDefinition(), (dataset[i].getResult()));
            } else if (!litterLocation[dataset[i].getLocation()]->contains(dataset[i].getDeterminandDefinition())) {
                litterLocation[dataset[i].getLocation()]->insert(dataset[i].getDeterminandDefinition(), (dataset[i].getResult()));
            } else {
                litterLocation[dataset[i].getLocation()]->insert(dataset[i].getDeterminandDefinition(), litterLocation[dataset[i].getLocation()]->value(dataset[i].getDeterminandDefinition()) + (dataset[i].getResult()));
            }

            // Pie chart data
            if (!litterWaterType.contains(dataset[i].getMaterialType())) {
                litterWaterType[dataset[i].getMaterialType()] = new QMap<QString, int>();
                litterWaterType[dataset[i].getMaterialType()]->insert(dataset[i].getDeterminandDefinition(), (dataset[i].getResult()));
            } else if (!litterWaterType[dataset[i].getMaterialType()]->contains(dataset[i].getDeterminandDefinition())) {
                litterWaterType[dataset[i].getMaterialType()]->insert(dataset[i].getDeterminandDefinition(), (dataset[i].getResult()));
            } else {
                litterWaterType[dataset[i].getMaterialType()]->insert(dataset[i].getDeterminandDefinition(), litterWaterType[dataset[i].getMaterialType()]->value(dataset[i].getDeterminandDefinition()) + (dataset[i].getResult()));
            }
        }

        if (dataset[i].isComplianceSample()) {
            complianceSamples.first++;
        } else {
            complianceSamples.second++;
        }

        tmpSampleLocations[dataset[i].getLocation()]++;
        tmpDeterminandCounts[dataset[i].getDeterminandDefinition()]++;
        sampleDates[dataset[i].getDateTime().date()]++;

        // Append the pollutant data with QDateTime and Result
        QDateTime dateTime = dataset[i].getDateTime();
        if (dateTime.isValid() && !qIsNaN(dataset[i].getResult())) {
            pollutantData[dataset[i].getDeterminand()].append(QPair<QDateTime, double>(dataset[i].getDateTime(), dataset[i].getResult()));
        }

        // Store data in samplingPointData
        samplingPointData[dataset[i].getLocation()].append(
            QPair<QString, double>(dataset[i].getDeterminand(), dataset[i].getResult())
        );

        if (isFluor) {
            // Store in fluor-specific structures
            fluorPollutantData[dataset[i].getDeterminand()].append(dataset[i].getResult());
            fluorLocations[dataset[i].getLocation()].append(
                QPair<QString, double>(dataset[i].getDeterminand(), dataset[i].getResult()));
        }
    }

    QVector<QPair<QString, int>> sortedLocations;
    for (auto i = tmpSampleLocations.constBegin(); i != tmpSampleLocations.constEnd(); i++) {
        sortedLocations.append(qMakePair(i.key(), i.value()));
    }

    // Sort by count in descending order
    std::sort(sortedLocations.begin(), sortedLocations.end(),
        [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
            return a.second > b.second;
    });

    // Take only top 5 for sampleLocations
    int n = qMin(5, sortedLocations.size());
    for (int i = 0; i < n; ++i) {
        sampleLocations[sortedLocations[i].first] = sortedLocations[i].second;
    }

    QVector<QPair<QString, int>> sortedDeterminands;
    for (auto i = tmpDeterminandCounts.constBegin(); i != tmpDeterminandCounts.constEnd(); i++) {
        sortedDeterminands.append(qMakePair(i.key(), i.value()));
    }

    // Sort by count in descending order
    std::sort(sortedDeterminands.begin(), sortedDeterminands.end(),
        [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
            return a.second > b.second;
    });

    // Take only top 5 for determinandCounts
    n = qMin(5, sortedDeterminands.size());
    for (int i = 0; i < n; ++i) {
        determinandCounts[sortedDeterminands[i].first] = sortedDeterminands[i].second;
    }

    locationCount = uniqueLocations.size();
    determinandCount = uniqueDeterminands.size();

    qDebug() << "Pollutant Data Processed: " << pollutantData.keys();
    qDebug() << "Time Series Data: " << pollutantData;
}

// Function to calculate top 5 locations with the highest result values for a selected pollutant
QVector<QPair<QString, double>> WaterModel::getTopLocationsByPollutant(const QString& pollutant) const
{
    QVector<QPair<QString, double>> topLocations;

    // Iterate through the data and gather concentrations for the selected pollutant
    for (int i = 0; i < rowCount(QModelIndex()); ++i) {  // Fixed rowCount() call
        QString currentPollutant = data(index(i, 4), Qt::DisplayRole).toString(); // Assuming pollutant is in column 4
        if (currentPollutant == pollutant) {
            QString location = data(index(i, 2), Qt::DisplayRole).toString(); // Assuming location is in column 2
            double concentration = data(index(i, 6), Qt::DisplayRole).toDouble(); // Assuming concentration is in column 6
            topLocations.append(qMakePair(location, concentration));
        }
    }

    // Sort by concentration in descending order (top concentration first)
    std::sort(topLocations.begin(), topLocations.end(), [](const QPair<QString, double>& a, const QPair<QString, double>& b) {
        return a.second > b.second; // Sort by concentration (second element of QPair)
    });

    // Return top 5 locations
    if (topLocations.size() > 5) {
        topLocations.resize(5); // Limit to top 5
    }

    return topLocations;
}

QVector<QPair<QDateTime, double>> WaterModel::getDataForPollutant(const QString& pollutant) const
{
    return pollutantData.value(pollutant);
}

QVector<QPair<QDateTime, double>> WaterModel::getTimeRangeData(const QString& pollutant, const QString& timeRange) const {
    QVector<QPair<QDateTime, double>> filteredData;

    // Determine time range
    QDateTime now = QDateTime::currentDateTime();
    QDateTime startTime;
    if (timeRange == "Last Week") {
        startTime = now.addDays(-7);
    } else if (timeRange == "Last Month") {
        startTime = now.addMonths(-1);
    } else if (timeRange == "Last Year") {
        startTime = now.addYears(-1);
    } else {
        startTime = QDateTime(); // All Time
    }

    // Filter data by pollutant and time range
    for (const auto& point : pollutantData.value(pollutant)) {
        if (point.first >= startTime) {
            filteredData.append(point);
        }
    }

    return filteredData;
}

