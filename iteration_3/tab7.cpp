// Sample implementation for the third card - Raw Data

#include <QtWidgets>
#include "styles.hpp"
#include "tab7.hpp"


Tab7::Tab7(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
  setupStyles();
  adjustColumnWidths();
  
  // Install event filter for Styles changes
  qApp->installEventFilter(this);
  
  // Connect to model changes to readjust columns when data changes
  connect(waterModel, SIGNAL(modelReset()), this, SLOT(adjustColumnWidths()));
  connect(waterModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(adjustColumnWidths()));
  connect(waterModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(adjustColumnWidths()));
}

bool Tab7::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
    setupStyles();
    return true;
  }
  return QWidget::eventFilter(watched, event);
}

void Tab7::createWidgets() 
{
  tableView = new QTableView;
  tableView->setModel(waterModel);
  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  tableFont.setPointSize(12);
  tableView->setFont(tableFont);
  
  // Table behavior settings
  tableView->setAlternatingRowColors(true);
  tableView->setSortingEnabled(true);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  tableView->setShowGrid(true);
  tableView->setObjectName("mainTable");
  tableView->setWordWrap(false);  // Prevent word wrapping to get accurate column widths
  
  // Header settings
  QHeaderView* horizontalHeader = tableView->horizontalHeader();
  horizontalHeader->setStretchLastSection(true);
  horizontalHeader->setSectionsClickable(true);
  horizontalHeader->setHighlightSections(true);
  horizontalHeader->setObjectName("tableHeader");
  horizontalHeader->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  
  // Setting minimum row height
  tableView->verticalHeader()->setDefaultSectionSize(40);
  tableView->verticalHeader()->setVisible(false);
  
  // Make the table take up all available space
  tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
} 

void Tab7::adjustColumnWidths()
{
  if (!waterModel) return;
  
  const int padding = 20;  // Additional padding for each column
  QFontMetrics metrics(tableView->font());
  QModelIndex parent;  // Empty model index for root items
  
  for (int column = 0; column < waterModel->columnCount(parent); ++column) {
    int maxWidth = 0;
    
    // Check header width
    QString headerText = waterModel->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();
    maxWidth = metrics.horizontalAdvance(headerText);
    
    // Check content width for first 100 rows
    for (int row = 0; row < 100; ++row) {
      QModelIndex idx = waterModel->index(row, column, parent);
      QString text = waterModel->data(idx, Qt::DisplayRole).toString();
      int width = metrics.horizontalAdvance(text);
      maxWidth = qMax(maxWidth, width);
    }
    
    // Set the column width to the maximum width plus padding
    tableView->setColumnWidth(column, maxWidth + padding * 2);
  }
}

void Tab7::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(tableView);
  layout->setContentsMargins(20, 20, 20, 20);
  layout->setSpacing(0);
  
  setLayout(layout);
}

void Tab7::setupStyles()
{
  QString styleSheet = QString(R"(
    QWidget {
      background-color: transparent;
      color: %1;
    }
    
    QTableView#mainTable {
      background-color: %2;
      border: 1px solid %3;
      border-radius: %4px;
      selection-background-color: %5;
      selection-color: %6;
      gridline-color: %7;
    }
    
    QHeaderView::section {
      background-color: %8;
      color: %9;
      padding: %10px;
      border: none;
      font-weight: bold;
    }
    
    QHeaderView::section:first {
      border-top-left-radius: %4px;
    }
    
    QHeaderView::section:hover {
      background-color: %11;
    }
    
    QTableView::item {
      padding: %12px;
      margin: 0;
      border: none;
    }
    
    QTableView::item:selected {
      background-color: %5;
      color: %6;
      padding: %12px;
      margin: 0;
      border: none;
    }
    
    QTableView::item:alternate {
      background-color: %13;
      padding: %12px;
      margin: 0;
      border: none;
    }
    
    QScrollBar:vertical {
      border: none;
      background-color: %2;
      width: 12px;
      margin: 0px;
    }
    
    QScrollBar::handle:vertical {
      background-color: %3;
      border-radius: %4px;
      min-height: 20px;
    }
    
    QScrollBar::handle:vertical:hover {
      background-color: %11;
    }
    
    QScrollBar:horizontal {
      border: none;
      background-color: %2;
      height: 12px;
      margin: 0px;
    }
    
    QScrollBar::handle:horizontal {
      background-color: %3;
      border-radius: %4px;
      min-width: 20px;
    }
    
    QScrollBar::handle:horizontal:hover {
      background-color: %11;
    }
  )")
  .arg(Styles::textColor())                    // %1
  .arg(Styles::backgroundColor())              // %2
  .arg(Styles::borderColor())                  // %3
  .arg(Styles::BORDER_RADIUS)                  // %4
  .arg(Styles::selectionBackground())          // %5
  .arg(Styles::selectionText())                // %6
  .arg(Styles::gridColor())                    // %7
  .arg(Styles::tableHeaderBackground())        // %8
  .arg(Styles::tableHeaderText())              // %9
  .arg(Styles::PADDING_MEDIUM)                 // %10
  .arg(Styles::menuHover())                    // %11
  .arg(Styles::PADDING_SMALL)                  // %12
  .arg(Styles::alternateRowBackground());      // %13

  this->setStyleSheet(styleSheet);
}
