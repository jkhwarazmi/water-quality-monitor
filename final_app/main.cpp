// Modified COMP2811 Coursework 2 sample solution: application entry point

#include <QtWidgets>
#include "window.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QTranslator trn;
  if (trn.load(QLocale::system(), "watermonitor", "_", ":/i18n")) {
    app.installTranslator(&trn);
  }

  WaterWindow window;
  window.show();

  return app.exec();
}