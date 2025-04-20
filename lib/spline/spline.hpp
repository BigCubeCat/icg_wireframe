#pragma once
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
    size_t m_count_points;                   // K
    size_t m_count_segmens;                  // N
    size_t m_count_edges;                    // M
    size_t m_count_edges_between_neighbors;  // M1

   public:
    explicit BSpline();

    std::vector<Point> operator()();

    void set_points(std::vector<double> u, std::vector<double> v);

    void set_count_segmens(size_t n);

    void set_count_edges(size_t m);

    void set_count_edges_neigh(size_t m1);
};
