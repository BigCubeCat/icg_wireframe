#include "editor.hpp"
#include <qlogging.h>

#include <QMouseEvent>
#include <vector>

#include "ui_editor.h"

Editor::Editor(QWidget* parent, DataModel* model)
    : QWidget(parent), m_ui(new Ui::Editor), m_data(model) {
    m_ui->setupUi(this);

    m_scene.setSceneRect(-100, -100, 200, 200);
    setup_axes();

    // Обработка кликов
    m_ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    m_ui->graphicsView->viewport()->installEventFilter(this);
    m_ui->graphicsView->setScene(&m_scene);

    connect(m_ui->mSpinBox, &QSpinBox::valueChanged, m_data, &DataModel::set_m);
    connect(m_ui->nSpinBox, &QSpinBox::valueChanged, m_data, &DataModel::set_n);
    connect(m_ui->m1SpinBox, &QSpinBox::valueChanged, m_data,
            &DataModel::set_m1);

    connect(m_data, &DataModel::redraw_spline, this, &Editor::updateSpline);

    connect(m_ui->applyButton, &QPushButton::clicked, this, &Editor::apply);
}

Editor::~Editor() {
    delete m_ui;
}

void Editor::setup_axes() {
    m_scene.addLine(-1000, 0, 1000, 0, QPen(Qt::green));
    m_scene.addLine(0, -1000, 0, 1000, QPen(Qt::green));
}

void Editor::addPoint(const QPointF& pos) {
    auto* point = new PointItem(pos.x(), pos.y());
    m_scene.addItem(point);
    m_points.append(point);
    connect(point, &PointItem::positionChanged, this, &Editor::updateSpline);
    connect(point, &PointItem::pointDeleted, this, &Editor::handlePointDeleted);
    updateSpline();
}

void Editor::handlePointDeleted(PointItem* point) {
    m_points.removeOne(point);
    updateSpline();
    k_updated();
}

void Editor::updateSpline() {
    // Удаляем старые линии
    qDeleteAll(m_spline_segments);
    qDeleteAll(m_spline);
    m_spline_segments.clear();
    m_spline.clear();
    const auto k = m_points.size();
    // Рисуем новые линии между точками
    for (int i = 1; i < k; ++i) {
        QPointF p1 = m_points[i - 1]->pos();
        QPointF p2 = m_points[i]->pos();
        auto* line = m_scene.addLine(p1.x(), p1.y(), p2.x(), p2.y(),
                                     QPen(Qt::darkCyan, 2));
        m_spline_segments.append(line);
    }
    const auto spline_points = m_data->spline();
    const auto k1 = static_cast<int>(spline_points.size());
    qDebug() << "k1=" << k1;
    for (int i = 1; i < k1; ++i) {
        auto p1 = spline_points[i - 1];
        auto p2 = spline_points[i];
        auto* line =
            m_scene.addLine(p1.x(), p1.y(), p2.x(), p2.y(), QPen(Qt::red, 2));
        m_spline.append(line);
    }
}

bool Editor::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress &&
        obj == m_ui->graphicsView->viewport()) {
        auto* mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() == Qt::LeftButton) {
            auto scene_pos = m_ui->graphicsView->mapToScene(mouse_event->pos());
            if (!m_scene.itemAt(scene_pos, QTransform())) {
                addPoint(scene_pos);
            }
        }
    }
    k_updated();
    return QWidget::eventFilter(obj, event);
}

void Editor::k_updated() {
    auto k = m_points.size();
    m_ui->kSpinBox->setValue(k);
    m_ui->applyButton->setEnabled(k >= 4);
}

void Editor::apply() {
    const auto k = m_points.size();
    std::vector<double> u(k);
    std::vector<double> v(k);
    for (int i = 0; i < k; ++i) {
        QPointF point = m_points[i]->pos();
        u[i] = point.x();
        v[i] = point.y();
    }
    qDebug() << "u.size = " << u.size();
    m_data->set_points(std::move(u), std::move(v));
}

void Editor::normalize() {}
