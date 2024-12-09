// Modified COMP2811 Coursework 2 sample solution: application entry point

#include <QtWidgets>
#include "window.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  // Create and show window
  WaterWindow window;
  window.show();

  return app.exec();
}