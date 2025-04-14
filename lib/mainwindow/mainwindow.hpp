#pragma once

#include "editor.hpp"

#include <QActionGroup>
#include <QMainWindow>
#include <qboxlayout.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent);
  ~MainWindow() override;

private:
  Editor m_editor;
  Ui::MainWindow *m_ui;

  QHBoxLayout m_tab1;
  QHBoxLayout m_tab2;

  void connectSlots();
};
