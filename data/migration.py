import sqlite3
import csv

# File paths
csv_file_path = 'data.csv'  # Replace with your CSV file path
sqlite_db_path = 'data.db'  # Replace with desired SQLite database file path

# Connect to SQLite database (creates it if it doesn't exist)
conn = sqlite3.connect(sqlite_db_path)
cursor = conn.cursor()

# Open the CSV file
with open(csv_file_path, 'r', encoding='utf-8') as csv_file:
    csv_reader = csv.reader(csv_file)
    headers = next(csv_reader)  # Read the header row
    
    # Create a table using headers
    table_name = 'WaterQuality'  # Choose a name for your table
    cursor.execute(f"CREATE TABLE {table_name} ( "
        "id                                         TEXT    PRIMARY KEY"
        "                                                   NOT NULL,"
        "[sample.samplingPoint]                     TEXT,"
        "[sample.samplingPoint.notation]            TEXT,"
        "[sample.samplingPoint.label]               TEXT,"
        "[sample.sampleDateTime]                    TEXT,"
        "[determinand.label]                        TEXT,"
        "[determinand.definition]                   TEXT,"
        "[determinand.notation]                     INTEGER,"
        "[resultQualifier.notation]                 TEXT,"
        "result                                     REAL,"
        "[codedResultInterpretation.interpretation] TEXT,"
        "[determinand.unit.label]                   TEXT,"
        "[sample.sampledMaterialType.label]         TEXT,"
        "[sample.isComplianceSample]                TEXT,"
        "[sample.purpose.label]                     TEXT,"
        "[sample.samplingPoint.easting]             INTEGER,"
        "[sample.samplingPoint.northing]            INTEGER"
    ");")
    
    # Insert rows into the table
    for row in csv_reader:
        placeholders = ', '.join(['?' for _ in row])
        cursor.execute(f'INSERT INTO {table_name} VALUES ({placeholders});', row)

# Commit changes and close the connection
conn.commit()
conn.close()

print(f"Data from {csv_file_path} has been successfully imported into {sqlite_db_path}.")
