#pragma once

#include <QActionGroup>
#include <QMainWindow>

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
  Ui::MainWindow *m_ui;
  void connectSlots();
};
