// Modified COMP2811 Coursework 1 sample solution: WaterDataset class

#pragma once

#include <string>

class WaterDataset
{
  public:
    WaterDataset() {}
    WaterDataset(const std::string &filename) { loadData(filename); }
    
    // Load data from CSV into SQLite database
    void loadData(const std::string& filename);
    
    // Get total number of records in database
    int size() const;
};