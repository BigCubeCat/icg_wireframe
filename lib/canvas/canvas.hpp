#pragma once

#include <eigen3/Eigen/Dense>

#include <QPainter>
#include <QWidget>
#include <vector>
#include "canvas_utils.hpp"
#include "model.hpp"

struct Vertex3D {
    double x, y, z;
    Vertex3D(double x, double y, double z) : x(x), y(y), z(z) {}
};

class Canvas : public QWidget {
    Q_OBJECT
   public slots:
    void pallete_changed(const QColor& a, const QColor& b);

   private:
    DataModel* m_data;

    double m_rotation_angle = 0;

    QColor m_farColor = Qt::red;
    QColor m_nearColor = Qt::blue;

    bool m_is_draging = false;
    QPoint m_begin_point;
    QPoint m_current_point;

    void update_rotation(const Point& prev, const Point& curr);

    void normalize();

    void draw_axes(QPainter& painter);

   public:
    explicit Canvas(QWidget* parent = nullptr, DataModel* model = nullptr);

    void paintEvent(QPaintEvent* event) override;

   protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
};
