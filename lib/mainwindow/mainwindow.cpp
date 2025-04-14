#include "mainwindow.hpp"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow), m_editor(nullptr),
      m_tab1(), m_tab2() {
  m_ui->setupUi(this);
  m_tab2.addWidget(&m_editor);
  m_ui->tab_2->setLayout(&m_tab2);
  connectSlots();
}

void MainWindow::connectSlots() {}

MainWindow::~MainWindow() { delete m_ui; }
