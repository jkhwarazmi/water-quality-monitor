// Header for Tab 7 - Help

#pragma once

#include <QWidget>
#include "model.hpp"

class QLabel;
class QScrollArea;
class QVBoxLayout;

class Tab7: public QWidget
{
  Q_OBJECT

  public:
    Tab7(WaterModel *model, QWidget *parent = nullptr);

  private:
    WaterModel* waterModel;
    QLabel *title;
    QScrollArea *scrollArea;
    QWidget *contentContainer;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
