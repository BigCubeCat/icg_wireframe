#pragma once

#include <QPainter>
#include <QWidget>
#include <eigen3/Eigen/Dense>
#include "model.hpp"

struct Vertex3D {
    double x, y, z;
    Vertex3D(double x, double y, double z) : x(x), y(y), z(z) {}
};

class Canvas : public QWidget {
    Q_OBJECT
   protected:
    void paintEvent(QPaintEvent*) override;

   private:
    DataModel* m_data;

    Eigen::Matrix4d m_rotation;
    Eigen::Matrix4d m_camera_translate;
    Eigen::Matrix4d m_camera_perspective;
    Eigen::Matrix4d m_translate;
    Eigen::Matrix4d m_normalize;

    double m_near_clip = 5.0;
    double m_rotation_angle = 5.0;

    double m_sh;
    double m_sw;

   public:
    explicit Canvas(QWidget* parent = nullptr, DataModel* model = nullptr);
};
