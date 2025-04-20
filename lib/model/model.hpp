#pragma once

#include <qobject.h>

#include "../spline/point.hpp"
#include "../spline/spline.hpp"

class DataModel : public QObject {
    Q_OBJECT
   public:
    explicit DataModel(QObject* parent = nullptr) : QObject(parent) {}
    void set_points(std::vector<double> u, std::vector<double> v);
    std::vector<Point> spline() const { return m_spline_points; };

   signals:
    void n_changed(int n);
    void m_changed(int m);
    void m1_changed(int m1);

    void redraw_spline();

   private:
    int m_n = 0;
    int m_m = 0;
    int m_m1 = 0;
    std::vector<Point> m_spline_points;

    BSpline m_spline;

   public slots:
    void set_n(int value);
    void set_m(int value);
    void set_m1(int value);
};
