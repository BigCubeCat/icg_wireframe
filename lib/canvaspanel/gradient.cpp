#include "gradient.hpp"
#include <QPainter>

void Grad::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, m_a);
    gradient.setColorAt(1, m_b);
    painter.fillRect(rect(), gradient);
}
