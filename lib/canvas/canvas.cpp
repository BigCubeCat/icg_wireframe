#include "canvas.hpp"

#include <qevent.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qpainterpath.h>
#include <qtpreprocessorsupport.h>
#include <qwidget.h>
#include <QTimer>

#include <algorithm>
#include <cmath>
#include "Eigen/src/Core/Matrix.h"
#include "canvas_utils.hpp"

const int kAxesSize = 50;

const double kZoomStep = 0.1;
const double kMaxZoom = 3;
const double kMinZoom = 0.3;

Canvas::Canvas(QWidget* parent, DataModel* model)
    : QWidget(parent), m_data(model) {
    m_point_cam = Eigen::Vector3d(-10, 0, 0);
    m_point_view = Eigen::Vector3d(10, 0, 0);
    m_vec_up = Eigen::Vector3d(0, 1, 0);
}

void Canvas::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    auto spline = m_data->spline();
    spline.calc_figure(m_data->m(), m_data->m1());
    auto fig = spline.figure();
    auto edges = spline.edges();

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // 1) Собираем view/proj матрицы
    auto view = make_view_matrix(m_point_cam, m_point_view, m_vec_up);
    auto proj = make_projection_matrix(m_zn, m_zf, m_sw, m_sh);

    // 2) Проецируем все точки
    std::vector<QPointF> pts2d;
    pts2d.reserve(fig.size());
    for (const auto& v : fig) {
        pts2d.push_back(project_point(v, view, proj, width(), height()));
    }

    // 3) Рисуем рёбра
    p.setPen(Qt::black);
    for (auto& e : edges) {
        const QPointF& a = pts2d[e.x()];
        const QPointF& b = pts2d[e.y()];
        p.drawLine(a, b);
    }
}

void Canvas::pallete_changed(const QColor& a, const QColor& b) {
    m_near_color = a;
    m_far_color = b;
    update();
}

void Canvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_begin_point = event->pos();
        m_is_draging = true;
    }
}

void Canvas::mouseMoveEvent(QMouseEvent* event) {
    if (m_is_draging) {
        m_current_point = event->pos();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_is_draging) {
        qDebug() << "mouseReleaseEvent()\n";
        m_is_draging = false;
    }
}

void Canvas::mouseDoubleClickEvent(QMouseEvent* event) {
    // TODO: reset rotation
}

void Canvas::wheelEvent(QWheelEvent* event) {
    const int delta = event->angleDelta().y();
    if (delta > 0) {
        m_zn = std::min(m_zn + kZoomStep, kMaxZoom);
    } else if (delta < 0) {
        m_zn = std::max(m_zn - kZoomStep, kMinZoom);
    }
    update();
}

void Canvas::draw_axes(QPainter& painter) {
    const QPoint center = QPoint(kAxesSize, kAxesSize);
    // X-axis (красный)
    painter.setPen(Qt::red);
    painter.drawLine(center, center + QPoint(kAxesSize, 0));

    // Y-axis (зеленый)
    painter.setPen(Qt::green);
    painter.drawLine(center, center + QPoint(0, kAxesSize));

    // Z-axis (синий)
    painter.setPen(Qt::blue);
    painter.drawLine(center, center - QPoint(kAxesSize, 0));
}
