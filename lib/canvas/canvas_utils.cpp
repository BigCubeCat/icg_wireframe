#include "canvas_utils.hpp"

#include <cmath>

QColor interpolate_color(const QColor& near, const QColor& far, double t) {
    t = qBound(0.0, t, 1.0);
    return QColor((far.red() * (1 - t)) + (near.red() * t),
                  (far.green() * (1 - t)) + (near.green() * t),
                  (far.blue() * (1 - t)) + (near.blue() * t));
}

Eigen::Matrix4d make_view_matrix(const Eigen::Vector3d& eye,
                                 const Eigen::Vector3d& center,
                                 const Eigen::Vector3d& up) {
    Eigen::Vector3d f = (center - eye).normalized();  // forward
    Eigen::Vector3d s = f.cross(up).normalized();     // right
    Eigen::Vector3d u = s.cross(f);                   // recomputed up

    Eigen::Matrix4d m = Eigen::Matrix4d::Identity();
    m.block<1, 3>(0, 0) = s.transpose();
    m.block<1, 3>(1, 0) = u.transpose();
    m.block<1, 3>(2, 0) = -f.transpose();
    m(0, 3) = -s.dot(eye);
    m(1, 3) = -u.dot(eye);
    m(2, 3) = f.dot(eye);
    return m;
}

Eigen::Matrix4d make_projection_matrix(double zn, double zf, double sw,
                                       double sh) {
    Eigen::Matrix4d p = Eigen::Matrix4d::Zero();
    p(0, 0) = 2 * zn / sw;
    p(1, 1) = 2 * zn / sh;
    p(2, 2) = -(zf + zn) / (zf - zn);
    p(2, 3) = -2 * zf * zn / (zf - zn);
    p(3, 2) = -1;
    return p;
}

Eigen::Matrix4d make_rotation_matrix(double x, double y) {
    double angle_x = x * M_PI / 180.0F;
    double angle_y = y * M_PI / 180.0F;

    Eigen::Matrix4d rot_x = Eigen::Matrix4d::Identity();
    rot_x(1, 1) = cos(angle_x);
    rot_x(1, 2) = -sin(angle_x);
    rot_x(2, 1) = sin(angle_x);
    rot_x(2, 2) = cos(angle_x);

    Eigen::Matrix4d rot_y = Eigen::Matrix4d::Identity();
    rot_y(0, 0) = cos(angle_y);
    rot_y(0, 2) = sin(angle_y);
    rot_y(2, 0) = -sin(angle_y);
    rot_y(2, 2) = cos(angle_y);

    return rot_y * rot_x;
}

QPointF project_point(const Point3D& v, const Eigen::Matrix4d& func, int width,
                      int height) {
    // Переводим в гомогенные координаты
    Eigen::Vector4d vh(v.x(), v.y(), v.z(), 1.0);

    // В camera-space, затем в clip-space
    Eigen::Vector4d clip = func * vh;

    // Перевод в NDC (Normalized Device Coordinates)
    clip /= clip.w();

    float x_ndc = clip.x();  // в [-1,1]
    float y_ndc = clip.y();  // в [-1,1]

    // В экранные: x->[0,W], y->[0,H] (инвертируем y)
    float x_screen = (x_ndc + 1.0) * 0.5 * width;
    float y_screen = (1.0 - y_ndc) * 0.5 * height;
    return QPointF(x_screen, y_screen);
}
