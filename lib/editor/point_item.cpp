#include "point_item.hpp"
#include <qbrush.h>
#include <qcolor.h>
#include <qnamespace.h>
#include <QGraphicsSceneMouseEvent>

const int kRadius = 7;

PointItem::PointItem(qreal x, qreal y, QGraphicsItem* parent)
    : QGraphicsEllipseItem(-kRadius, -kRadius, 2 * kRadius, 2 * kRadius,
                           parent) {
    setPos(x, y);
    setBrush(QBrush(Qt::green));
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
}

void PointItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        emit pointDeleted(this);
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
