#pragma once

#include <qobject.h>

#include "../spline/point.hpp"
#include "../spline/spline.hpp"

class DataModel : public QObject {
    Q_OBJECT
   public:
    explicit DataModel(QObject* parent = nullptr) : QObject(parent) {}

    void set_k(size_t value);
    void set_n(size_t value);
    void set_m(size_t value);
    void set_m1(size_t value);

   signals:
    void parameters_changed(size_t k, size_t n, size_t m, size_t m1);

   private:
    size_t m_k = 4;
    size_t m_n = 0;
    size_t m_m = 0;
    size_t m_m1 = 0;
    std::vector<Point> m_spline_points;

    BSpline m_spline;

    void set_parameter(size_t& parameter, size_t value);
};
