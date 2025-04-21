#include "mainwindow.hpp"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_cavnas(nullptr, &m_model),
      m_editor(nullptr, &m_model),
      m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_tab2.addWidget(&m_editor);
    m_ui->tab_2->setLayout(&m_tab2);
    m_tab1.addWidget(&m_cavnas);
    m_ui->tab->setLayout(&m_tab1);
    connectSlots();
}

void MainWindow::connectSlots() {}

MainWindow::~MainWindow() {
    delete m_ui;
}
