// Application entry point

#include <QtWidgets>
#include "window.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  // Set up database connection - THIS IS VERY SLOW
  // Also SQL does not handle scrolling through 144k rows well
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("../data.db");

  if (!db.open()) {
    QMessageBox::critical(nullptr, "Database Error", "Could not open database: " + db.lastError().text());
    return -1;
  }

  // Create and show window
  WaterWindow window;
  window.show();

  return app.exec();
}