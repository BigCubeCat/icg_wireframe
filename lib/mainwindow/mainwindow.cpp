#include "mainwindow.hpp"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_model(this),
      m_editor(nullptr, &m_model),
      m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_tab2.addWidget(&m_editor);
    m_ui->tab_2->setLayout(&m_tab2);
    connectSlots();
}

void MainWindow::connectSlots() {}

MainWindow::~MainWindow() {
    delete m_ui;
}
