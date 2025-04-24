#include "canvas.hpp"

#include <qlogging.h>
#include <qnamespace.h>
#include <qtpreprocessorsupport.h>
#include <qwidget.h>
#include <QTimer>

#include <algorithm>
#include <cmath>
#include "Eigen/src/Core/Matrix.h"

const int kViewSize = 200;

Canvas::Canvas(QWidget* parent, DataModel* model)
    : QWidget(parent),
      m_data(model),
      m_rotation(Eigen::Matrix4d::Identity()),
      m_translate(Eigen::Matrix4d::Identity()) {
    m_camera_perspective << 2 * m_near_clip, 0, 0, 0, 0, 2 * m_near_clip, 0, 0,
        0, 0, 1, 0, 0, 0, 1, 0;
    qDebug() << "Canvas() " << width() << " " << height() << isVisible();
    setVisible(true);
}

void Canvas::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    const auto center_x = width() / 2;
    const auto center_y = height() / 2;
    QPainter painter(this);
    painter.fillRect(rect(), Qt::yellow);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::darkBlue, 3));
    for (size_t i = 0; i < m_edges.size(); ++i) {
        const auto point_a = m_figure_points[m_edges[i]];
        const auto point_b = m_figure_points[m_edges[i + 1]];
        qDebug() << center_x + (point_a.x() * kViewSize) << " "
                 << center_y - (point_a.y() * kViewSize) << " "
                 << center_x + (point_b.x() * kViewSize) << " "
                 << center_y - (point_b.y() * kViewSize);
        painter.drawLine(center_x + (point_a.x() * kViewSize),
                         center_y - (point_a.y() * kViewSize),
                         center_x + (point_b.x() * kViewSize),
                         center_y - (point_b.y() * kViewSize));
    }
}

void Canvas::pallete_changed(const QColor& a, const QColor& b) {
    m_close = a;
    m_far = b;
    update();
}

void Canvas::update_rotation(const Point& prev, const Point& curr) {
    Eigen::Vector4d axis(-(curr.y() - prev.y()), -(curr.x() - prev.x()), 0,
                         1.0);
    axis.normalize();

    double c = cos(m_rotation_angle);
    double s = sin(m_rotation_angle);
    double x = axis.x();
    double y = axis.y();
    double z = axis.z();

    auto rot = Eigen::Matrix4d();
    rot << c + ((1 - c) * x * x), ((1 - c) * x * y) - (s * z),
        ((1 - c) * x * z) + (s * y), 0, ((1 - c) * y * x) + (s * z),
        c + ((1 - c) * y * y), ((1 - c) * y * z) - (s * x), 0,
        ((1 - c) * z * x) - (s * y), ((1 - c) * z * y) + (s * x),
        c + ((1 - c) * z * z), 0, 0, 0, 0, 1.0;

    m_translate = rot * m_translate;
    m_rotation = rot * m_rotation;
}

void Canvas::normalize() {
    auto spline = m_data->spline();
    auto max = spline.max_value();
    auto min = spline.min_value();

    m_sw = max[0];
    m_sh = max[1];

    double dist_x = max[0] - min[0];
    double dist_y = max[1] - min[1];
    double dist_z = max[2] - min[2];

    double res = std::max({dist_x, dist_y, dist_z});
    if (std::abs(res) <= 1.e-6)
        res = 1.0;

    m_normalize = Eigen::Matrix4d();
    double tmp = 1.0 / res;
    m_normalize << tmp, 0, 0, 0, 0, tmp, 0, 0, 0, 0, tmp, 0, 0, 0, 0, 1;
}

void Canvas::create_points() {
    auto spline = m_data->spline();

    auto spline_points = m_data->spline_points();

    auto m = m_data->m();
    auto n = m_data->n();
    auto m1 = m_data->m1();
    const auto k = spline_points.size();
    const auto count_segments = m * n;

    double angle = static_cast<double>(360) / count_segments;

    normalize();
    m_figure_points.clear();
    m_figure_points.reserve(count_segments * k);
    m_edges.clear();
    m_edges.reserve(count_segments * k * 3);

    for (size_t i = 0; i < count_segments; i++) {
        auto current_angle = i * angle * M_PI / 180;
        auto cosinus = cos(current_angle);
        auto sinus = cos(current_angle);

        for (const auto& point : spline_points) {
            Eigen::Vector4d canvas_point(point.y() * cosinus, point.y() * sinus,
                                         point.x(), 1.0);
            canvas_point = m_translate * canvas_point;
            canvas_point = m_camera_translate * canvas_point;
            canvas_point = m_normalize * canvas_point;
            m_figure_points.push_back(canvas_point);
        }
    }

    for (size_t i = 0; i < count_segments; i += n) {
        for (size_t j = 0; j < k - 1; j++) {
            m_edges.push_back(j + (i * k));
            m_edges.push_back(j + 1 + (i * k));
        }
    }
    for (size_t i = 0; i < count_segments; ++i) {
        m_edges.push_back(i * k);
        m_edges.push_back(((i + 1) % (count_segments)) * k);
    }

    for (size_t i = 0; i < count_segments; i++) {
        m_edges.push_back((i * k) + k - 1);
        m_edges.push_back((((i + 1) % (count_segments)) * k) + k - 1);
    }

    if (m1 > 2) {
        int step = k / (m1 - 1);
        for (size_t i = 1; i <= m1 - 2; i++) {
            for (size_t j = 0; j < count_segments; j++) {
                m_edges.push_back((j * k) + (i * step));
                m_edges.push_back((((j + 1) % (count_segments)) * k) +
                                  (i * step));
            }
        }
    }
    update();
}
