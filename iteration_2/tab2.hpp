// Sample header for second first card - Tab1

#pragma once

#include <QWidget>
#include "model.hpp"

class QLabel;

class Tab2: public QWidget
{
  Q_OBJECT

  public:
    Tab2(WaterModel *model, QWidget *parent = nullptr);

  private:
    WaterModel* waterModel;
    QLabel *placeholder;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
