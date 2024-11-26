// Sample implementation for the third card - Raw Data

#include <QtWidgets>
#include "tab3.hpp"


Tab3::Tab3(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
  createWidgets();
  createLayout();
  setupStyles();
  adjustColumnWidths();
  
  // Install event filter for theme changes
  qApp->installEventFilter(this);
  
  // Connect to model changes to readjust columns when data changes
  connect(waterModel, SIGNAL(modelReset()), this, SLOT(adjustColumnWidths()));
  connect(waterModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(adjustColumnWidths()));
  connect(waterModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(adjustColumnWidths()));
}

bool Tab3::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
    setupStyles();
    return true;
  }
  return QWidget::eventFilter(watched, event);
}

void Tab3::createWidgets() 
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

void Tab3::adjustColumnWidths()
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
    
    // Check content width for each row
    for (int row = 0; row < waterModel->rowCount(parent); ++row) {
      QModelIndex idx = waterModel->index(row, column, parent);
      QString text = waterModel->data(idx, Qt::DisplayRole).toString();
      int width = metrics.horizontalAdvance(text);
      maxWidth = qMax(maxWidth, width);
    }
    
    // Set the column width to the maximum width plus padding
    tableView->setColumnWidth(column, maxWidth + padding * 2);
  }
}

void Tab3::createLayout()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(tableView);
  layout->setContentsMargins(20, 20, 20, 20);
  layout->setSpacing(0);
  
  setLayout(layout);
}

void Tab3::setupStyles()
{
  bool isDarkMode = qApp->palette().color(QPalette::Window).lightness() < 128;
  
  // Define colors based on theme
  QString textColor = isDarkMode ? "#ffffff" : "#2c3e50";
  QString bgColor = isDarkMode ? "#2c3e50" : "#ffffff";
  QString altRowColor = isDarkMode ? "#34495e" : "#f8f9fa";
  QString headerBgColor = isDarkMode ? "#3498db" : "#007bff";
  QString headerTextColor = "#ffffff";
  QString borderColor = isDarkMode ? "#4a6480" : "#dee2e6";
  QString selectionColor = isDarkMode ? "#3498db" : "#007bff";
  QString selectionTextColor = "#ffffff";
  QString gridColor = isDarkMode ? "#4a6480" : "#dee2e6";
  
  QString styleSheet = QString(R"(
    QWidget {
      background-color: transparent;
      color: %1;
    }
    
    QTableView#mainTable {
      background-color: %2;
      border: 1px solid %3;
      border-radius: 6px;
      selection-background-color: %8;
      selection-color: %9;
      gridline-color: %10;
    }
    
    QHeaderView::section {
      background-color: %4;
      color: %5;
      padding: 10px;
      border: none;
      font-weight: bold;
    }
    
    QHeaderView::section:first {
      border-top-left-radius: 6px;
    }
    
    QHeaderView::section:hover {
      background-color: %6;
    }
    
    QTableView::item {
      padding: 8px;
      margin: 0;
      border: none;
    }
    
    QTableView::item:selected {
      background-color: %8;
      color: %9;
      padding: 8px;
      margin: 0;
      border: none;
    }
    
    /* Style for alternating rows */
    QTableView::item:alternate {
      background-color: %7;
      padding: 8px;
      margin: 0;
      border: none;
    }
    
    /* Scrollbar styling */
    QScrollBar:vertical {
      border: none;
      background-color: %2;
      width: 12px;
      margin: 0px;
    }
    
    QScrollBar::handle:vertical {
      background-color: %3;
      border-radius: 6px;
      min-height: 20px;
    }
    
    QScrollBar::handle:vertical:hover {
      background-color: %6;
    }
    
    QScrollBar:horizontal {
      border: none;
      background-color: %2;
      height: 12px;
      margin: 0px;
    }
    
    QScrollBar::handle:horizontal {
      background-color: %3;
      border-radius: 6px;
      min-width: 20px;
    }
    
    QScrollBar::handle:horizontal:hover {
      background-color: %6;
    }
  )")
  .arg(textColor)          // %1 text color
  .arg(bgColor)           // %2 background color
  .arg(borderColor)       // %3 border color
  .arg(headerBgColor)     // %4 header background
  .arg(headerTextColor)   // %5 header text
  .arg(selectionColor)    // %6 hover/selection color
  .arg(altRowColor)       // %7 alternate row color
  .arg(selectionColor)    // %8 selection background
  .arg(selectionTextColor) // %9 selection text
  .arg(gridColor);        // %10 grid color

  this->setStyleSheet(styleSheet);
}
