#include "canvas_utils.hpp"

QColor interpolate_color(const QColor& near, const QColor& far, double t) {
    t = qBound(0.0, t, 1.0);
    return QColor((far.red() * (1 - t)) + (near.red() * t),
                  (far.green() * (1 - t)) + (near.green() * t),
                  (far.blue() * (1 - t)) + (near.blue() * t));
}

Eigen::Matrix4d compute_model_view_matrix(double angle) {
    Eigen::Matrix4d matrix = Eigen::Matrix4d::Identity();
    matrix.block<3, 3>(0, 0) =
        Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitY()).toRotationMatrix();
    return matrix;
}

Eigen::Matrix4d compute_ortho_projection(double left, double right,
                                         double bottom, double top) {
    Eigen::Matrix4d matrix = Eigen::Matrix4d::Identity();
    matrix(0, 0) = 2.0 / (right - left);
    matrix(1, 1) = 2.0 / (top - bottom);
    matrix(0, 3) = -(right + left) / (right - left);
    matrix(1, 3) = -(top + bottom) / (top - bottom);
    return matrix;
}

std::vector<Vertex> generate_surface(const std::vector<double>& x,
                                     const std::vector<double>& y, int slices) {
    std::vector<Vertex> vertices;
    for (int i = 0; i < slices; ++i) {
        double theta = 2 * M_PI * i / slices;
        for (size_t j = 0; j < x.size(); ++j) {
            vertices.push_back({x[j] * cos(theta), y[j], x[j] * sin(theta)});
        }
    }
    return vertices;
}
