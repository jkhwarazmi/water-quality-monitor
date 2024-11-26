// Sample implementation for the table view card - Raw Data

#include <QtWidgets>
#include "tab3.hpp"


Tab3::Tab3(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
}

void Tab3::createWidgets() 
{
  tableView = new QTableView;
  tableView->setModel(waterModel);
  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  tableView->setFont(tableFont);
  tableView->setAlternatingRowColors(true);
  tableView->setSortingEnabled(true);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->verticalHeader()->setVisible(false);
} 

void Tab3::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(tableView);
  
  setLayout(layout);
}