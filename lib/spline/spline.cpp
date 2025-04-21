#include "spline.hpp"

#include <cstddef>
#include <eigen3/Eigen/Dense>

#include <iostream>
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
    std::cout << m_matrix_m << std::endl;
};

std::vector<Point> BSpline::spline_points() {
    return m_spline_points;
};

void BSpline::operator()() {
    m_spline_points.clear();
    m_spline_points.reserve(m_count_points * m_count_segmens);

    m_x_i.reserve(m_count_points * m_count_segmens);
    m_y_i.reserve(m_count_points * m_count_segmens);
    m_z_i.reserve(m_count_points * m_count_segmens);

    double step = 1.0 / static_cast<double>(m_count_segmens);

    Eigen::Matrix<double, 1, 4> t_vector;
    for (size_t i = 1; i < m_count_points - 2; ++i) {
        Eigen::Map<Eigen::Vector4d> u_vector(m_points_u.data() +
                                             static_cast<ptrdiff_t>(i - 1));
        Eigen::Map<Eigen::Vector4d> v_vector(m_points_v.data() +
                                             static_cast<ptrdiff_t>(i - 1));
        auto tmp_u = m_matrix_m * u_vector;
        auto tmp_v = m_matrix_m * v_vector;

        for (size_t k = 0; k <= m_count_segmens; ++k) {
            double t = step * k;
            get_t_vector(t_vector, t);
            auto x = (t_vector * tmp_u).coeff(0) / 6;
            auto y = (t_vector * tmp_v).coeff(0) / 6;
            m_spline_points.emplace_back(x, y);
            m_x_i.push_back(x);
            m_y_i.push_back(y);
            m_z_i.push_back(x);
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
