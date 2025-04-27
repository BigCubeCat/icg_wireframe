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

Canvas::Canvas(QWidget* parent, DataModel* model)
    : QWidget(parent), m_data(model) {}

void Canvas::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    auto spline = m_data->spline();
    spline.calc_figure();
    auto x = spline.x();
    auto y = spline.y();
    auto z = spline.z();

    const size_t slices = 36;  // Количество сегментов вращения
    const size_t size = x.size();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), Qt::white);

    // Параметры
    const int m = m_data->m();          // Количество меридианов (сплайнов)
    const double rotation_speed = 0.5;  // Скорость вращения

    // Матрицы преобразований
    Eigen::Matrix4d view_matrix;
    view_matrix << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 5,  // Сдвиг камеры по Z
        0, 0, 0, 1;

    Eigen::Matrix4d projection_matrix;
    const double fov = M_PI / 4;
    projection_matrix << 1 / (tan(fov / 2)), 0, 0, 0, 0, 1 / (tan(fov / 2)), 0,
        0, 0, 0, -1, -1, 0, 0, -1, 0;

    // Структура для хранения линий с цветом
    struct Line {
        QPointF p1;
        QPointF p2;
        double avgDepth;
        QColor color;
    };

    std::vector<Line> lines;
    const double angle_step = 2 * M_PI / m;

    // Генерация линий для всех меридианов
    for (int i = 0; i < m; ++i) {
        const double angle = (angle_step * i) + m_rotation_angle;
        const Eigen::Matrix3d rotation =
            Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitZ()).matrix();

        // Преобразуем и проектируем точки
        std::vector<QPointF> screen_points;
        std::vector<double> depths;

        for (size_t j = 0; j < size; ++j) {
            Eigen::Vector3d point(x[j], y[j], 0);
            Eigen::Vector3d rotated = rotation * point;
            Eigen::Vector4d view_space = view_matrix * rotated.homogeneous();
            Eigen::Vector4d clip_space = projection_matrix * view_space;

            // Перспективное деление
            const double w = clip_space.w();
            QPointF screen_point((clip_space.x() / w + 1.0) * width() / 2,
                                 (1.0 - clip_space.y() / w) * height() / 2);

            screen_points.push_back(screen_point);
            depths.push_back(view_space.z());
        }

        // Создаем линии между точками
        for (size_t j = 0; j < screen_points.size() - 1; ++j) {
            const double avg_depth = (depths[j] + depths[j + 1]) / 2;
            lines.push_back({screen_points[j], screen_points[j + 1], avg_depth,
                             Qt::transparent});
            qDebug() << "AddLine";
        }
    }

    // Находим диапазон глубин
    auto [minDepth, maxDepth] = std::minmax_element(
        lines.begin(), lines.end(),
        [](const Line& a, const Line& b) { return a.avgDepth < b.avgDepth; });
    // Присваиваем цвета
    const double depth_range = maxDepth->avgDepth - minDepth->avgDepth;
    for (Line& line : lines) {
        line.color = interpolate_color(
            m_nearColor, m_farColor,
            (line.avgDepth - minDepth->avgDepth) / depth_range);
    }

    // Сортировка линий по глубине (дальние -> ближние)
    std::sort(lines.begin(), lines.end(), [](const Line& a, const Line& b) {
        return a.avgDepth > b.avgDepth;
    });

    // Отрисовка линий
    painter.setPen(Qt::NoPen);
    for (const Line& line : lines) {
        qDebug() << line.p1 << " " << line.p2;
        QPainterPath path;
        path.moveTo(line.p1);
        path.lineTo(line.p2);

        QPen pen(line.color, 1.5);
        pen.setCosmetic(true);  // Фиксированная толщина
        painter.setPen(pen);
        painter.drawPath(path);
    }

    // Обновление угла для анимации
    m_rotation_angle += 0.005;
    draw_axes(painter);  // Реализуйте отдельно при необходимости
    update();
}

void Canvas::pallete_changed(const QColor& a, const QColor& b) {
    m_nearColor = a;
    m_farColor = b;
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
