#include "editor.hpp"

#include "ui_editor.h"

Editor::Editor(QWidget *parent) : QWidget(parent), m_ui(new Ui::Editor) {
  m_ui->setupUi(this);
}

Editor::~Editor() { delete m_ui; }
