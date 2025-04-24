#pragma once

#include <eigen3/Eigen/Dense>

#include <QPainter>
#include <QWidget>
#include <vector>
#include "model.hpp"

struct Vertex3D {
    double x, y, z;
    Vertex3D(double x, double y, double z) : x(x), y(y), z(z) {}
};

class Canvas : public QWidget {
    Q_OBJECT
   public slots:
    void pallete_changed(const QColor& a, const QColor& b);
    void create_points();

   private:
    DataModel* m_data;

    Eigen::Matrix4d m_rotation;
    Eigen::Matrix4d m_camera_translate;
    Eigen::Matrix4d m_camera_perspective;
    Eigen::Matrix4d m_translate;
    Eigen::Matrix4d m_normalize;

    std::vector<Eigen::Vector4d> m_figure_points;
    std::vector<double> m_edges;

    double m_near_clip = 5.0;
    double m_rotation_angle = 5.0;

    double m_sh;
    double m_sw;

    QColor m_far = Qt::blue;
    QColor m_close = Qt::red;

    void update_rotation(const Point& prev, const Point& curr);

    void normalize();

   public:
    explicit Canvas(QWidget* parent = nullptr, DataModel* model = nullptr);

    void paintEvent(QPaintEvent* event) override;
};
