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

    size_t n() const { return m_n; }

    size_t m() const { return m_m; }

    size_t m1() const { return m_m1; }

    std::pair<std::vector<double>, std::vector<double>> points() {
        return m_spline.points();
    }

    void add_point(double x, double y);

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
