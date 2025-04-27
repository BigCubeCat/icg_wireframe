#pragma once

#include <Eigen/Dense>
#include <QColor>
#include <QPoint>

#include "../spline/point.hpp"

QColor interpolate_color(const QColor& near, const QColor& far, double t);

Eigen::Matrix4d make_view_matrix(const Eigen::Vector3d& eye,
                                 const Eigen::Vector3d& center,
                                 const Eigen::Vector3d& up);

Eigen::Matrix4d make_projection_matrix(double zn, double zf, double sw,
                                       double sh);

QPointF project_point(const Point3D& v, const Eigen::Matrix4d& view,
                      const Eigen::Matrix4d& proj, int width, int height);
