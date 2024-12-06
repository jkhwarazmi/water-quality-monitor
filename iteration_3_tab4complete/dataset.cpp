// Modified COMP2811 Coursework 1 sample solution: WaterDataset class

#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "dataset.hpp"
#include "csv.hpp"

using namespace std;


void WaterDataset::loadData(const string& filename) {
  try {
    csv::CSVReader reader(filename);
      
    data.clear();
      
    // Verify required columns exist
    const auto& headers = reader.get_col_names();
    const vector<string> required_columns = {
      "sample.samplingPoint.easting",
      "sample.samplingPoint.northing",
      "sample.samplingPoint.label",
      "sample.sampleDateTime",
      "determinand.label",
      "determinand.definition",
      "result",
      "determinand.unit.label",
      "sample.sampledMaterialType.label",
      "sample.isComplianceSample",
      "sample.purpose.label"
    };
        
    for (const auto& col : required_columns) {
      if (find(headers.begin(), headers.end(), col) == headers.end()) {
        throw runtime_error("Missing required column: " + col);
      }
    }
        
    int lineNo = 2;
    for (const auto& row: reader) {
      try {
        Sample sample{
          row["sample.samplingPoint.easting"].get<int>(),
          row["sample.samplingPoint.northing"].get<int>(),
          QString::fromStdString(row["sample.samplingPoint.label"].get<string>()),
          QString::fromStdString(row["sample.sampleDateTime"].get<string>()),
          QString::fromStdString(row["determinand.label"].get<string>()),
          QString::fromStdString(row["determinand.definition"].get<string>()),
          row["result"].get<double>(),
          QString::fromStdString(row["determinand.unit.label"].get<string>()),
          QString::fromStdString(row["sample.sampledMaterialType.label"].get<string>()),
          row["sample.isComplianceSample"].get<string>() == "true",
          QString::fromStdString(row["sample.purpose.label"].get<string>())
        };
          
        data.push_back(sample);
          
      } catch (const exception& e) {
        // Exclude invalid rows
        qWarning() << "Error parsing row" << lineNo << ":" << e.what() << "Row skipped.";
      }
      lineNo++;
    }
        
    qInfo() << "Successfully loaded" << data.size() << "samples from" << QString::fromStdString(filename);
    
  } catch (const exception& e) {
    qCritical() << "Error loading data:" << e.what();
    throw; 
  }
}

void WaterDataset::checkDataExists() const
{
  if (size() == 0) {
    throw runtime_error("Dataset is empty!");
  }
}
