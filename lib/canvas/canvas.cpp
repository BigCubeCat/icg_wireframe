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
        ts.push_back(depth);
        max = std::max(max, depth);
        min = std::min(min, depth);
        pts2d.push_back(new_point);
    }

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
    draw_axes(p);
}

void Canvas::pallete_changed(const QColor& a, const QColor& b) {
    m_near_color = a;
    m_far_color = b;
    m_data->m_near = m_near_color;
    m_data->m_far = m_far_color;

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
        m_data->m_rot_x = m_rotation_x;
        m_data->m_rot_y = m_rotation_y;
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_is_draging) {
        m_is_draging = false;
    }
}

void Canvas::mouseDoubleClickEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    m_zn = kDefaultZn;
    m_rotation_x = 0;
    m_rotation_y = 0;
    m_data->m_zn = m_zn;
    m_data->m_rot_x = m_rotation_x;
    m_data->m_rot_y = m_rotation_y;
    update();
}

void Canvas::wheelEvent(QWheelEvent* event) {
    const int delta = event->angleDelta().y();
    if (delta > 0) {
        m_zn = std::min(m_zn + kZoomStep, kMaxZoom);
    } else if (delta < 0) {
        m_zn = std::max(m_zn - kZoomStep, kMinZoom);
    }
    m_data->m_zn = m_zn;
    update();
}

void Canvas::update_from_data() {
    m_rotation_x = m_data->m_rot_x;
    m_rotation_y = m_data->m_rot_y;
    m_zn = m_data->m_zn;
    m_far_color = m_data->m_far;
    m_near_color = m_data->m_near;
}

void Canvas::draw_axes(QPainter& painter) {
    const QPoint center = QPoint(kAxesSize, kAxesSize);

    // Углы вращения в радианах
    double alpha = qDegreesToRadians(m_rotation_x);  // Вращение вокруг X
    double beta = qDegreesToRadians(-m_rotation_y);  // Вращение вокруг Y

    // Вычисляем синусы и косинусы
    double cos_alpha = cos(alpha);
    double sin_alpha = sin(alpha);
    double cos_beta = cos(beta);
    double sin_beta = sin(beta);

    // Проекция 3D точки в 2D (x_proj = x - z, y_proj = y)
    auto project = [](double x, double y, double z) -> QPointF {
        return QPointF(x - z, y);
    };

    // Вычисляем координаты концов осей после вращений
    // Ось X (красный)
    painter.setPen(Qt::red);
    QPointF x_axis = project(cos_beta,              // X после вращения вокруг Y
                             sin_beta * sin_alpha,  // Y после вращения вокруг X
                             -sin_beta * cos_alpha  // Z после вращения вокруг X
                             ) *
                     kAxesSize;
    painter.drawLine(center, center + x_axis.toPoint());

    // Ось Y (зеленый)
    painter.setPen(Qt::green);
    QPointF y_axis = project(0,          // X остаётся 0
                             cos_alpha,  // Y после вращения вокруг X
                             sin_alpha   // Z после вращения вокруг X
                             ) *
                     kAxesSize;
    painter.drawLine(center, center + y_axis.toPoint());

    // Ось Z (синий)
    painter.setPen(Qt::blue);
    QPointF z_axis =
        project(sin_beta,               // X после вращения вокруг Y
                -cos_beta * sin_alpha,  // Y после вращения вокруг X
                cos_beta * cos_alpha    // Z после вращения вокруг X
                ) *
        kAxesSize;
    painter.drawLine(center, center + z_axis.toPoint());
}
