// Sample header for the table view card - Raw Data

#pragma once

#include <QWidget>
#include "model.hpp"

class QTableView;
class QLabel;

class Tab3 : public QWidget
{
  Q_OBJECT

  public:
    Tab3(WaterModel* model, QWidget* parent = nullptr);

  private:
    WaterModel* waterModel;
    QTableView* tableView;

    void createWidgets();
    void createLayout();
};