#pragma once

#include <QActionGroup>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Editor;
}
QT_END_NAMESPACE

class Editor : public QWidget {
  Q_OBJECT

public:
  explicit Editor(QWidget *parent);
  ~Editor() override;

private:
  Ui::Editor *m_ui;
};
