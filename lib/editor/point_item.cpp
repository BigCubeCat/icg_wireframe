#include "point_item.hpp"
#include <qbrush.h>
#include <qnamespace.h>
#include <QGraphicsSceneMouseEvent>

PointItem::PointItem(qreal x, qreal y, QGraphicsItem* parent)
    : QGraphicsEllipseItem(-5, -5, 10, 10, parent) {
    setPos(x, y);
    setBrush(QBrush(Qt::cyan));
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
}

void PointItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        emit pointDeleted(this);
        delete this;
    } else {
        QGraphicsEllipseItem::mousePressEvent(event);
    }
}

QVariant PointItem::itemChange(GraphicsItemChange change,
                               const QVariant& value) {
    if (change == ItemPositionHasChanged) {
        emit positionChanged();
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}
