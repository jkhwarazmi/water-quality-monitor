// Sample header for the first card - Tab1

#pragma once

#include <QWidget>
#include "model.hpp"

class QPushButton;
class QLabel;

class Tab1: public QWidget
{
  Q_OBJECT

  public:
    Tab1(WaterModel* model, QWidget* parent = nullptr);

  signals:
    void reloadRequested();

  private:
    WaterModel* waterModel;
    QPushButton *reloadButton;
    QLabel *placeholder;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};