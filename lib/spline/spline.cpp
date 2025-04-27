#include "spline.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <eigen3/Eigen/Dense>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>
#include "Eigen/src/Core/Matrix.h"
#include "point.hpp"

namespace {
void get_t_vector(Eigen::Matrix<double, 1, 4>& vec, double t) {
    double scalar = 1;
    for (int i = 0; i < 4; ++i) {
        vec[3 - i] = scalar;
        scalar *= t;
    }
}
};  // namespace

BSpline::BSpline() {
    m_matrix_m << -1, 3, -3, 1, 3, -6, 3, 0, -3, 0, 3, 0, 1, 4, 1, 0;
};

std::vector<Point2D> BSpline::spline_points() {
    return m_spline_points;
};

void BSpline::operator()(double x, double y) {
    m_points_u.push_back(x);
    m_points_v.push_back(y);

    double index = m_points_u.size() - 4;
    Eigen::Map<Eigen::Vector4d> u_vector(m_points_u.data() +
                                         static_cast<ptrdiff_t>(index));
    Eigen::Map<Eigen::Vector4d> v_vector(m_points_v.data() +
                                         static_cast<ptrdiff_t>(index));
    Eigen::Matrix<double, 1, 4> t_vector;
    auto tmp_u = m_matrix_m * u_vector;
    auto tmp_v = m_matrix_m * v_vector;
    for (size_t k = 0; k <= m_count_segmens; ++k) {
        double t = m_step * k;
        get_t_vector(t_vector, t);
        auto x = (t_vector * tmp_u).coeff(0) / 6;
        auto y = (t_vector * tmp_v).coeff(0) / 6;
        m_spline_points.emplace_back(x, y);
    }
}

void BSpline::operator()() {
    m_spline_points.clear();
    m_spline_points.reserve(m_count_points * m_count_segmens);

    Eigen::Matrix<double, 1, 4> t_vector;
    for (size_t i = 1; i < m_count_points - 2; ++i) {
        Eigen::Map<Eigen::Vector4d> u_vector(m_points_u.data() +
                                             static_cast<ptrdiff_t>(i - 1));
        Eigen::Map<Eigen::Vector4d> v_vector(m_points_v.data() +
                                             static_cast<ptrdiff_t>(i - 1));
        auto tmp_u = m_matrix_m * u_vector;
        auto tmp_v = m_matrix_m * v_vector;

        for (size_t k = 0; k <= m_count_segmens; ++k) {
            double t = m_step * k;
            get_t_vector(t_vector, t);
            auto x = (t_vector * tmp_u).coeff(0) / 6;
            auto y = (t_vector * tmp_v).coeff(0) / 6;
            m_spline_points.emplace_back(x, y);
        }
    }
}

void BSpline::set_points(std::vector<double> u, std::vector<double> v) {
    m_points_u = std::move(u);
    m_points_v = std::move(v);
    if (m_points_u.size() != m_points_v.size()) {
        throw std::runtime_error("BSpline: sizes mismatch");
    }
    m_count_points = m_points_u.size();
}

void BSpline::set_count_segmens(size_t n) {
    m_count_segmens = n;
    m_step = 1.0 / static_cast<double>(m_count_segmens);
}

void BSpline::set_count_edges(size_t m) {
    m_count_edges = m;
}

void BSpline::set_count_edges_neigh(size_t m1) {
    m_count_edges_between_neighbors = m1;
}

std::pair<std::vector<double>, std::vector<double>> BSpline::points() {
    return {m_points_u, m_points_v};
}

void BSpline::calc_figure(int m, int m1) {
    m_maximum = {0, 0, 0};
    m_minimum = {
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
    };

    const double phi = 2.0 * M_PI / static_cast<double>(m);
    // производим нормализацию
    const auto size = m_spline_points.size();
    m_figure.clear();
    m_figure.reserve(size * m * m1);
    for (size_t i = 0; i < size; ++i) {  // по точкам сплайна
        for (int j = 0; j < m; ++j) {
            auto phi_angle = phi * i;
            m_figure.emplace_back(m_spline_points[i].y() * cos(phi_angle),
                                  m_spline_points[i].y() * sin(phi_angle),
                                  m_spline_points[i].x());
            const double theta = phi / m1;
            for (int k = 1; k < m1; ++k) {
                auto theta_angle = phi_angle + (theta * k);
                m_figure.emplace_back(m_spline_points[i].y() * cos(theta_angle),
                                      m_spline_points[i].y() * sin(theta_angle),
                                      m_spline_points[i].x());
            }
        }
    }
    m_edges.reserve((size * m * (m1 + 1)));
    for (size_t i = 0; i < size - 1; ++i) {  // точки сплайна
        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < m1; ++k) {
                m_edges.emplace_back(i * m * m1, (i + 1) * m * m1);
            }
        }
    }
}
