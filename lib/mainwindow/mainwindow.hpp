#pragma once

#include "canvas.hpp"
#include "editor.hpp"
#include "fileprocessor.hpp"
#include "model.hpp"

#include <qboxlayout.h>
#include <QActionGroup>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
   private slots:
    void open();
    void save();
    void save_as();

   public:
    explicit MainWindow(QWidget* parent);
    ~MainWindow() override;

   private:
    DataModel m_model;
    Editor m_editor;
    Canvas m_cavnas;
    FileProcessor m_fp;
    Ui::MainWindow* m_ui;

    QHBoxLayout m_tab1;
    QHBoxLayout m_tab2;

    void connectSlots();
};
