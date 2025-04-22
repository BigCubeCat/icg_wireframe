#include "scalable_graphics_view.hpp"
#include <QApplication>
#include <QWheelEvent>

ScalableGraphicsView::ScalableGraphicsView(QWidget* parent)
    : QGraphicsView(parent) {
    // Настройки для улучшения визуализации
    setRenderHint(QPainter::Antialiasing, true);

    // Масштабирование относительно курсора
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Режим перемещения сцены при зажатой ЛКМ
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ScalableGraphicsView::wheelEvent(QWheelEvent* event) {
    // Коэффициент масштабирования
    const double scaleFactor = 1.15;
    double factor =
        (event->angleDelta().y() > 0) ? scaleFactor : 1.0 / scaleFactor;

    // Текущий масштаб
    QTransform transform = this->transform();
    double currentScale = transform.m11();  // X-масштаб

    // Ограничения масштаба
    const double minScale = 0.1;
    const double maxScale = 10.0;

    if ((currentScale * factor < minScale) ||
        (currentScale * factor > maxScale)) {
        return;  // Не выходим за границы
    }

    // Применяем масштабирование
    scale(factor, factor);
    event->accept();
}
