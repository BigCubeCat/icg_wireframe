#pragma once

#include <QGraphicsEllipseItem>
#include <QObject>

class PointItem : public QObject, public QGraphicsEllipseItem {
  Q_OBJECT
public:
  explicit PointItem(qreal x, qreal y, QGraphicsItem *parent = nullptr);

signals:
  void positionChanged();
  void pointDeleted(PointItem *);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) override;
};
