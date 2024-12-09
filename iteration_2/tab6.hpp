// [OPTIONAL] Header for sixth card - Geographical Hotspots

#pragma once

#include <QWidget>
#include "model.hpp"

class QLabel;

class Tab6: public QWidget
{
  Q_OBJECT

  public:
    Tab6(WaterModel *model, QWidget *parent = nullptr);

  private:
    WaterModel* waterModel;
    QLabel *placeholder;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
