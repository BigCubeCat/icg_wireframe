#include <QApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QWidget>

#include "mainwindow.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow view(nullptr);
  view.show();
  QApplication::exec();

  return 0;
}
