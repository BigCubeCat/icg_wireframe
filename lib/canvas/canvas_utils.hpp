#pragma once

#include <Eigen/Dense>
#include <QColor>
#include <QPoint>
#include <array>

/// Структура для хранения преобразованных вершин и глубины
struct TransformedVertex {
    QPointF pos;   /// 2D экранные координаты
    double depth;  /// Глубина в 3D пространстве
    QColor color;  /// Рассчитанный цвет
};

/// Группировка в треугольники и расчет цветов
struct Triangle {
    std::array<QPointF, 3> points;
    double avg_depth;
    QColor color;
};

struct Vertex {
    double x, y, z;
};

QColor interpolate_color(const QColor& near, const QColor& far, double t);

Eigen::Matrix4d compute_model_view_matrix(double angle);

Eigen::Matrix4d compute_ortho_projection(double left, double right,
                                         double bottom, double top);

std::vector<Vertex> generate_surface(const std::vector<double>& x,
                                     const std::vector<double>& y, int slices);
