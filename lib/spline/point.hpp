#pragma once

#include <array>

struct Point {
    std::array<double, 2> m_data{0, 0};
    Point() = default;
    Point(double x, double y) {
        m_data[0] = x;
        m_data[1] = y;
    }
    double x() const { return m_data[0]; }
    double y() const { return m_data[1]; }
    void set_x(double x) { m_data[0] = x; }
    void set_y(double y) { m_data[1] = y; }
};
