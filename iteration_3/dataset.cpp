// WaterDataset class implementation using SQLite

#include <stdexcept>
#include <algorithm>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include "dataset.hpp"
#include "csv.hpp"

using namespace std;


void WaterDataset::loadData(const string &filename) {
  try {
    // Open database connection
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
      throw runtime_error("Invalid database connection");
    }

    // Create table if it doesn't exist
    QSqlQuery createQuery;
    if (!createQuery.exec(
      "CREATE TABLE IF NOT EXISTS WaterQuality ("
      "id TEXT PRIMARY KEY NOT NULL, "
      "[sample.samplingPoint] TEXT, "
      "[sample.samplingPoint.notation] TEXT, "
      "[sample.samplingPoint.label] TEXT, "
      "[sample.sampleDateTime] TEXT, "
      "[determinand.label] TEXT, "
      "[determinand.definition] TEXT, "
      "[determinand.notation] INTEGER," 
      "[resultQualifier.notation] TEXT, "
      "result REAL, "
      "[codedResultInterpretation.interpretation] TEXT, "
      "[determinand.unit.label] TEXT, "
      "[sample.sampledMaterialType.label] TEXT, "
      "[sample.isComplianceSample] INTEGER, " 
      "[sample.purpose.label] TEXT, "
      "[sample.samplingPoint.easting] INTEGER, "
      "[sample.samplingPoint.northing] INTEGER"
      ")"
    )) {
      throw runtime_error("Failed to create table: " + createQuery.lastError().text().toStdString());
    }

    // Start transaction
    if (!db.transaction()) {
      throw runtime_error("Could not start transaction: " + db.lastError().text().toStdString());
    }

    // Verify required columns exist
    csv::CSVReader reader(filename);
    const auto &headers = reader.get_col_names();
    const vector<string> required_columns = {
      "@id",
      "sample.samplingPoint",
      "sample.samplingPoint.notation",
      "sample.samplingPoint.label",
      "sample.sampleDateTime",
      "determinand.label",
      "determinand.definition",
      "determinand.notation",
      "resultQualifier.notation",
      "result",
      "codedResultInterpretation.interpretation",
      "determinand.unit.label",
      "sample.sampledMaterialType.label",
      "sample.isComplianceSample",
      "sample.purpose.label",
      "sample.samplingPoint.easting",
      "sample.samplingPoint.northing"
    };

    for (const auto &col : required_columns) {
      if (find(headers.begin(), headers.end(), col) == headers.end()) {
        throw runtime_error("Missing required column: " + col);
      }
    }

    // Clear existing data
    QSqlQuery clearQuery("DELETE FROM WaterQuality", db);
    if (!clearQuery.exec()) {
      throw runtime_error("Failed to clear existing data: " + clearQuery.lastError().text().toStdString());
    }

    // Prepare the insert statement
    QSqlQuery query(db);
    query.prepare(
      "INSERT INTO WaterQuality ("
      "id, "
      "[sample.samplingPoint], "
      "[sample.samplingPoint.notation], "
      "[sample.samplingPoint.label], "
      "[sample.sampleDateTime], "
      "[determinand.label], "
      "[determinand.definition], "
      "[determinand.notation], "
      "[resultQualifier.notation], "
      "result, "
      "[codedResultInterpretation.interpretation], "
      "[determinand.unit.label], "
      "[sample.sampledMaterialType.label], "
      "[sample.isComplianceSample], "
      "[sample.purpose.label], "
      "[sample.samplingPoint.easting], "
      "[sample.samplingPoint.northing]"
      ") VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
    );

    int lineNo = 2;
    int successCount = 0;
    int batchSize = 1000; 
      
    for (const auto& row: reader) {
      try {
        query.bindValue(0, QString::fromStdString(row["@id"].get<string>()));
        query.bindValue(1, QString::fromStdString(row["sample.samplingPoint"].get<string>()));
        query.bindValue(2, QString::fromStdString(row["sample.samplingPoint.notation"].get<string>()));
        query.bindValue(3, QString::fromStdString(row["sample.samplingPoint.label"].get<string>()));
        query.bindValue(4, QString::fromStdString(row["sample.sampleDateTime"].get<string>()));
        query.bindValue(5, QString::fromStdString(row["determinand.label"].get<string>()));
        query.bindValue(6, QString::fromStdString(row["determinand.definition"].get<string>()));
        query.bindValue(7, row["determinand.notation"].get<int>());
        query.bindValue(8, QString::fromStdString(row["resultQualifier.notation"].get<string>()));
        query.bindValue(9, row["result"].get<double>());
        query.bindValue(10, QString::fromStdString(row["codedResultInterpretation.interpretation"].get<string>()));
        query.bindValue(11, QString::fromStdString(row["determinand.unit.label"].get<string>()));
        query.bindValue(12, QString::fromStdString(row["sample.sampledMaterialType.label"].get<string>()));
        query.bindValue(13, row["sample.isComplianceSample"].get<string>() == "true" ? 1 : 0);
        query.bindValue(14, QString::fromStdString(row["sample.purpose.label"].get<string>()));
        query.bindValue(15, row["sample.samplingPoint.easting"].get<int>());
        query.bindValue(16, row["sample.samplingPoint.northing"].get<int>());

        if (!query.exec()) {
          throw runtime_error("Query execution failed: " + query.lastError().text().toStdString());
        }

        successCount++;

        // Commit in batches to avoid memory issues
        if (successCount % batchSize == 0) {
          if (!db.commit()) {
            throw runtime_error("Failed to commit batch: " + db.lastError().text().toStdString());
          }
          if (!db.transaction()) {
            throw runtime_error("Failed to start new transaction: " + db.lastError().text().toStdString());
          }
        }

      } catch (const exception& e) {
        qWarning() << "Error parsing row" << lineNo << ":" << e.what() << "Row skipped.";
      }
      lineNo++;
    }

    // Commit final batch
    if (!db.commit()) {
      throw runtime_error("Failed to commit final batch: " + db.lastError().text().toStdString());
    }

    qInfo() << "Successfully loaded " << successCount << "samples from " << QString::fromStdString(filename);

  } catch (const exception& e) {
    // Rollback on any error
    QSqlDatabase::database().rollback();
    qCritical() << "Error loading data:" << e.what();
    throw;
  }
}

int WaterDataset::size() const {
  QSqlQuery query("SELECT COUNT(*) FROM WaterQuality");
        
  if (!query.exec() || !query.next()) {
    throw runtime_error("Failed to check data existence");
  }
  return query.value(0).toInt();
}
