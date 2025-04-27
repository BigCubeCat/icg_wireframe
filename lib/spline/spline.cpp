#include "spline.hpp"

#include <algorithm>
#include <cstddef>
#include <eigen3/Eigen/Dense>

#include <iostream>
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

std::vector<Point> BSpline::spline_points() {
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
        m_x_i.push_back(x);
        m_y_i.push_back(y);
        m_z_i.push_back(x);
    }
}

void BSpline::operator()() {
    m_spline_points.clear();
    m_x_i.clear();
    m_y_i.clear();
    m_z_i.clear();
    m_spline_points.reserve(m_count_points * m_count_segmens);

    m_x_i.reserve(m_count_points * m_count_segmens);
    m_y_i.reserve(m_count_points * m_count_segmens);
    m_z_i.reserve(m_count_points * m_count_segmens);
    m_maximum = {0, 0, 0};
    m_minimum = {
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
    };

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
            m_minimum = {
                std::min(m_minimum[0], x),
                std::min(m_minimum[1], y),
                std::min(m_minimum[2], x),
            };
            m_maximum = {
                std::max(m_maximum[0], x),
                std::max(m_maximum[1], y),
                std::max(m_maximum[2], x),
            };
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

void BSpline::calc_figure() {
    const auto size = m_x_i.size();
    std::vector<double> figure_size;
    std::transform(m_maximum.begin(), m_maximum.end(), m_minimum.begin(),
                   std::back_inserter(figure_size), std::minus());
    const double max_size =
        *std::max_element(figure_size.begin(), figure_size.end()) / 2;
    // производим нормализацию
    for (size_t i = 0; i < size; ++i) {
        m_x_i[i] = (m_x_i[i] / max_size);
        m_y_i[i] = (m_y_i[i] / max_size);
        m_z_i[i] = (m_z_i[i] / max_size);
    }
}
