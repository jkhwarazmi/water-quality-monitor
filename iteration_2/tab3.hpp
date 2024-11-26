// Sample header for third card - Tab3

#pragma once

#include <QWidget>
#include "model.hpp"

class QLabel;

class Tab3: public QWidget
{
  Q_OBJECT

  public:
    Tab3(WaterModel *model, QWidget *parent = nullptr);

  private:
    WaterModel* waterModel;
    QLabel *placeholder;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
