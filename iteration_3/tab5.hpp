// Header for fifth card - Compliance
// Sara

#pragma once

#include <QWidget>
#include "model.hpp"

class QLabel;

class Tab5: public QWidget
{
  Q_OBJECT

  public:
    Tab5(WaterModel *model, QWidget *parent = nullptr);

  private:
    WaterModel* waterModel;
    QLabel *placeholder;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
