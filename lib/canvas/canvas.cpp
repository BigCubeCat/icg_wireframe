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
#include <limits>
#include "Eigen/src/Core/Matrix.h"
#include "canvas_utils.hpp"

const int kAxesSize = 50;

const double kZoomStep = 0.1;
const double kMaxZoom = 3;
const double kMinZoom = 0.3;
const double kSensitivity = 0.5;  // чувствительность

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
    auto rot = make_rotation_matrix(m_rotation_x, m_rotation_y);
    auto func = view * rot;

    // 2) Проецируем все точки
    std::vector<QPointF> pts2d;
    pts2d.reserve(fig.size());
    std::vector<double> ts;
    ts.reserve(fig.size());
    double max = std::numeric_limits<double>::min();
    double min = std::numeric_limits<double>::max();
    for (const auto& v : fig) {
        double depth;
        auto new_point = project_point(v, func, proj, width(), height(), depth);
        qDebug() << depth;
        ts.push_back(depth);
        max = std::max(max, depth);
        min = std::min(min, depth);
        pts2d.push_back(new_point);
    }
    const auto norm = max + min;
    qDebug() << "norm=" << norm;

    // 3) Рисуем рёбра
    for (auto& e : edges) {
        int i0 = e.x();
        int i1 = e.y();
        const QPointF& a = pts2d[i0];
        const QPointF& b = pts2d[i1];

        // Цвета на концах
        QColor c0 =
            interpolate_color(m_near_color, m_far_color, ts[i0], min, max);
        QColor c1 =
            interpolate_color(m_near_color, m_far_color, ts[i1], min, max);

        // Градиент от A к B
        QLinearGradient grad(a, b);
        grad.setColorAt(0.0, c0);
        grad.setColorAt(1.0, c1);

        QPen pen;
        pen.setBrush(grad);
        pen.setWidth(2);  // при необходимости
        p.setPen(pen);

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
        QPoint delta = event->pos() - m_begin_point;
        m_begin_point = event->pos();
        // Вращаем на основе перемещения мыши
        m_rotation_x += kSensitivity * delta.y();
        m_rotation_y += kSensitivity * delta.x();
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_is_draging) {
        qDebug() << "mouseReleaseEvent()\n";
        m_is_draging = false;
    }
}

void Canvas::mouseDoubleClickEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    m_zn = kDefaultZn;
    m_rotation_x = 0;
    m_rotation_y = 0;
    update();
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
