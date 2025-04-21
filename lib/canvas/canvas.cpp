#include "canvas.hpp"

#include <qwidget.h>
#include <QTimer>

Canvas::Canvas(QWidget* parent, DataModel* model)
    : QWidget(parent),
      m_data(model),
      m_rotation(Eigen::Matrix4d::Identity()),
      m_translate(Eigen::Matrix4d::Identity()) {
    m_camera_perspective << 2 * m_near_clip, 0, 0, 0, 0, 2 * m_near_clip, 0, 0,
        0, 0, 1, 0, 0, 0, 1, 0;
}

void Canvas::paintEvent(QPaintEvent*) {}
