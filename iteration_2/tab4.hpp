// Sample header for fourth card - Tab14

#pragma once

#include <QWidget>
#include "model.hpp"

class QLabel;

class Tab4: public QWidget
{
  Q_OBJECT

  public:
    Tab4(WaterModel *model, QWidget *parent = nullptr);

  private:
    WaterModel* waterModel;
    QLabel *placeholder;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
