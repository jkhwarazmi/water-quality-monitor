// Modified COMP2811 Coursework 2 solution: data model

#pragma once

#include <QAbstractTableModel>
#include "dataset.hpp"

class WaterModel: public QAbstractTableModel 
{
  Q_OBJECT

  public:
    WaterModel(QObject* parent = nullptr): QAbstractTableModel(parent) {}
      
    void updateFromFile(const QString&);
    bool hasData() const { return dataset.size() > 0; }
    double getPollutantThreshold(const QString&) const;
      
    int rowCount(const QModelIndex& index) const { return dataset.size(); }
    int columnCount(const QModelIndex& index) const { return 11; }
    
    QVariant data(const QModelIndex&, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;

  private:
    WaterDataset dataset;
};