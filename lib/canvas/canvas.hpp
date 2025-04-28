#pragma once

#include <eigen3/Eigen/Dense>

#include <QPainter>
#include <QWidget>
#include "model.hpp"

const double kDefaultZn = 1.0;

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
    double m_zn = kDefaultZn;  // ближняя плоскость
    double m_zf = 10.0;        // дальняя плоскость
    double m_sw = 2.0;         // ширина ближней плоскости
    double m_sh = 2.0;         // высота ближней плоскости

    QColor m_far_color = Qt::red;
    QColor m_near_color = Qt::blue;

    bool m_is_draging = false;
    QPoint m_begin_point;
    double m_rotation_x = 0.0;  // угол поворота вокруг X
    double m_rotation_y = 0.0;  // угол поворота вокруг Y

    void normalize();

    void draw_axes(QPainter& painter);

   public:
    explicit Canvas(QWidget* parent = nullptr, DataModel* model = nullptr);

    void paintEvent(QPaintEvent* event) override;

    void update_from_data();

   protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
};
