#pragma once

#include <array>

template <typename T, int Dimension>
struct Point {
    std::array<T, Dimension> m_data{};
    Point() = default;
    Point(T x, T y) {
        m_data[0] = x;
        m_data[1] = y;
    }
    Point(T x, T y, T z) {
        m_data[0] = x;
        m_data[1] = y;
        m_data[2] = z;
    }
    Point(T x, T y, T z, T w) {
        m_data[0] = x;
        m_data[1] = y;
        m_data[2] = z;
        m_data[3] = w;
    }
    T x() const { return m_data[0]; }
    T y() const { return m_data[1]; }
    T z() const { return m_data[2]; }
    T w() const { return m_data[3]; }
    void set_x(T x) { m_data[0] = x; }
    void set_y(T y) { m_data[1] = y; }
    void set_z(T z) { m_data[2] = z; }
    void set_w(T w) { m_data[3] = w; }
};

using Point2D = Point<double, 2>;
using Point3D = Point<double, 3>;
using Point2I = Point<size_t, 2>;
