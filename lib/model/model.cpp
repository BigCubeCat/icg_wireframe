#include "model.hpp"

void DataModel::set_parameter(size_t& parameter, size_t value) {
    if (parameter != value) {
        parameter = value;
        m_spline_points = m_spline();
    }
}

void DataModel::set_k(size_t value) {}

void DataModel::set_n(size_t value) {
    if (m_n != value) {
        m_n = value;
        m_spline_points = m_spline();
        m_spline.set_count_segmens(m_n);
        emit parameters_changed(m_k, m_n, m_m, m_m1);
    }
}

void DataModel::set_m(size_t value) {
    if (m_m != value) {
        m_m = value;
        m_spline_points = m_spline();
        m_spline.set_count_edges(m_m);
        emit parameters_changed(m_k, m_n, m_m, m_m1);
    }
}

void DataModel::set_m1(size_t value) {
    if (m_m1 != value) {
        m_m1 = value;
        m_spline_points = m_spline();
        m_spline.set_count_edges_neigh(m_m1);
        emit parameters_changed(m_k, m_n, m_m, m_m1);
    }
}
