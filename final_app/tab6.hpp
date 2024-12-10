// Header for Tab 6 - Raw Data

#pragma once

#include <QWidget>
#include "model.hpp"

class QTableView;

class Tab6 : public QWidget
{
  Q_OBJECT

  public:
    Tab6(WaterModel* model, QWidget* parent = nullptr);

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
