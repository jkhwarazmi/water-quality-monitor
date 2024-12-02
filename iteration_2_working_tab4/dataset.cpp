// Modified COMP2811 Coursework 1 sample solution: WaterDataset class

#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "dataset.hpp"
#include "csv.hpp"

using namespace std;



void WaterDataset::loadData(const string& filename) {
    try {
        qDebug() << "Starting to load data from file:" << QString::fromStdString(filename);

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

        int lineNo = 2;  // Start from the second line after headers
        int validRows = 0, skippedRows = 0;

        for (const auto& row : reader) {
            try {
                // Construct a Sample object
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
                validRows++;

            } catch (const exception& e) {
                qWarning() << "Error parsing row" << lineNo << ":" << e.what() << "Row skipped.";
                skippedRows++;
            }
            lineNo++;
        }

        qInfo() << "Finished loading data from file:" << QString::fromStdString(filename);
        qInfo() << "Total rows processed:" << (validRows + skippedRows);
        qInfo() << "Valid rows:" << validRows;
        qInfo() << "Skipped rows:" << skippedRows;

        if (validRows == 0) {
            throw runtime_error("No valid rows found in the dataset. Please check the data file.");
        }

    } catch (const exception& e) {
        qCritical() << "Error loading data:" << e.what();
        throw;  // Re-throw the exception after logging
    }
}

void WaterDataset::checkDataExists() const
{
  if (size() == 0) {
    throw runtime_error("Dataset is empty!");
  }
}
