#include "mainwindow.hpp"

#include "ui_mainwindow.h"

#include <QObject>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_editor(nullptr, &m_model),
      m_canvas_panel(nullptr, &m_model),
      m_fp(&m_model),
      m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_tab2.addWidget(&m_editor);
    m_ui->tab_2->setLayout(&m_tab2);
    m_tab1.addWidget(&m_canvas_panel);
    m_ui->tab->setLayout(&m_tab1);

    m_ui->toolBar->addActions(m_ui->menuFile->actions());

    connectSlots();
    open_file();
}

void MainWindow::connectSlots() {
    connect(m_ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(m_ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(m_ui->actionSaveAs, &QAction::triggered, this,
            &MainWindow::save_as);
}

void MainWindow::open() {
    m_fp.open();
    m_editor.open_spline();
}

void MainWindow::save() {
    m_fp.save();
}

void MainWindow::save_as() {
    m_fp.save_as();
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::open_file() {
    m_fp.read_file("assets/default.bin");
    m_editor.open_spline();
}
