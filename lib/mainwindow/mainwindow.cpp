#include "mainwindow.hpp"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow) {
  m_ui->setupUi(this);
  connectSlots();
}

void MainWindow::connectSlots() {}

MainWindow::~MainWindow() { delete m_ui; }
