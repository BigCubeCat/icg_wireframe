#include "model.hpp"
#include <qdebug.h>

void DataModel::set_n(int value) {
    if (m_n != value) {
        m_n = value;
        m_spline.set_count_segmens(m_n);
        emit n_changed(m_n);
        emit redraw_spline();
    }
}

void DataModel::set_m(int value) {
    if (m_m != value) {
        m_m = value;
        m_spline.set_count_edges(m_m);
        emit m_changed(m_m);
    }
}

void DataModel::set_m1(int value) {
    if (m_m1 != value) {
        m_m1 = value;
        m_spline.set_count_edges_neigh(m_m1);
        emit m1_changed(m_m1);
    }
}

void DataModel::set_points(std::vector<double> u, std::vector<double> v) {
    m_spline.set_points(std::move(u), std::move(v));
    m_spline_points = m_spline();
    emit redraw_spline();
}
