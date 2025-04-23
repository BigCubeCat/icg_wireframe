#pragma once
#include <utility>
#include <vector>
#include "eigen3/Eigen/Dense"
#include "point.hpp"

/*!
 * \brief Класс B-сплайна
*/
class BSpline {
   private:
    Eigen::Matrix4d m_matrix_m;
    std::vector<double> m_points_u;
    std::vector<double> m_points_v;

    size_t m_count_points = 0;                   // K
    size_t m_count_segmens = 2;                  // N
    size_t m_count_edges = 1;                    // M
    size_t m_count_edges_between_neighbors = 1;  // M1

    std::vector<Point> m_spline_points;
    std::vector<double> m_x_i;
    std::vector<double> m_y_i;
    std::vector<double> m_z_i;

    double m_step = 0.5;

   public:
    explicit BSpline();

    std::pair<std::vector<double>, std::vector<double>> points();

    std::vector<Point> spline_points();

    /// полностью пересчитвыаем сплайн
    void operator()();

    /// добавляем ОДНУ точку
    void operator()(double x, double y);

    void set_points(std::vector<double> u, std::vector<double> v);

    void set_count_segmens(size_t n);

    void set_count_edges(size_t m);

    void set_count_edges_neigh(size_t m1);
};
