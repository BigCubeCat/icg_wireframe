#include "canvaspanel.hpp"

#include <qwidget.h>
#include "canvas.hpp"

#include <QColorDialog>

#include "ui_canvaspanel.h"

CanvasPanel::CanvasPanel(QWidget* parent, DataModel* model)
    : QWidget(parent),
      m_canvas(this, model),
      m_data(model),
      m_gradient(this),
      m_ui(new Ui::CanvasPanel) {
    m_ui->setupUi(this);
    m_ui->layout->addWidget(&m_canvas);
    m_ui->wgt->addWidget(&m_gradient);

    connect(m_ui->pushButton_2, &QPushButton::clicked, this,
            &CanvasPanel::on_top_clicked);

    connect(m_ui->pushButton, &QPushButton::clicked, this,
            &CanvasPanel::on_bottom_clicked);
    //
    // connect(m_data, &DataModel::redraw_spline, &m_canvas,
    //         &Canvas::create_points);
}

CanvasPanel::~CanvasPanel() {
    delete m_ui;
}

void CanvasPanel::on_top_clicked() {
    auto new_color = ask_color(m_gradient.a());
    if (new_color.has_value()) {
        m_gradient.set_a_color(new_color.value());
        m_canvas.pallete_changed(m_gradient.b(), m_gradient.a());
    }
}

void CanvasPanel::on_bottom_clicked() {
    auto new_color = ask_color(m_gradient.b());
    if (new_color.has_value()) {
        m_gradient.set_b_color(new_color.value());
        m_canvas.pallete_changed(m_gradient.b(), m_gradient.a());
    }
}

std::optional<QColor> CanvasPanel::ask_color(const QColor& old) {
    QColor new_color = QColorDialog::getColor(old, this, "Choose color");
    if (new_color.isValid()) {
        return new_color;
    }
    return std::nullopt;
}
