// Sample header for the table view card - Raw Data

#pragma once

#include <QWidget>
#include "model.hpp"

class QTableView;

class Tab3 : public QWidget
{
  Q_OBJECT

  public:
    Tab3(WaterModel* model, QWidget* parent = nullptr);

  private slots:
    void adjustColumnWidths();

  private:
    WaterModel* waterModel;
    QTableView* tableView;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
