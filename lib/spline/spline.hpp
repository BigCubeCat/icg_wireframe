#pragma once
#include <array>
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

    std::vector<Point2D> m_spline_points;
    std::vector<Point3D> m_figure;
    std::vector<Point2I> m_edges;

    double m_step = 0.5;

    double m_maximum = 1;
    double m_minimum = 0;

   public:
    explicit BSpline();

    double depth() const { return m_maximum - m_minimum; }

    std::vector<Point3D> figure() const { return m_figure; }

    std::vector<Point2I> edges() const { return m_edges; }

    std::pair<std::vector<double>, std::vector<double>> points();

    std::vector<Point2D> spline_points();

    /// полностью пересчитвыаем сплайн
    void operator()();

    /// добавляем ОДНУ точку
    void operator()(double x, double y);

    void calc_figure(int m, int m1);

    void set_points(std::vector<double> u, std::vector<double> v);

    void set_count_segmens(size_t n);

    void set_count_edges(size_t m);

    void set_count_edges_neigh(size_t m1);
};
