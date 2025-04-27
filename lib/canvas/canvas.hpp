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

    // Положение камеры
    Eigen::Vector3d m_point_cam;
    // Точка, на которую смотрим
    Eigen::Vector3d m_point_view;
    // Вектор “вверх”
    Eigen::Vector3d m_vec_up;

    // Параметры пирамиды видимости
    double m_zn = 1.0f;    // ближняя плоскость
    double m_zf = 100.0f;  // дальняя плоскость
    double m_sw = 2.0f;    // ширина ближней плоскости
    double m_sh = 2.0f;    // высота ближней плоскости

    QColor m_far_color = Qt::red;
    QColor m_near_color = Qt::blue;

    bool m_is_draging = false;
    QPoint m_begin_point;
    QPoint m_current_point;

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
