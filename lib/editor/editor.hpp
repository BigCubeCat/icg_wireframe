#pragma once

#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <QActionGroup>
#include <QWidget>
#include "point_item.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class Editor;
}
QT_END_NAMESPACE

class Editor : public QWidget {
    Q_OBJECT
   private:
    Ui::Editor* m_ui;
    void setup_axes();

    QGraphicsScene m_scene;
    QList<PointItem*> m_points;
    QList<QGraphicsItem*> m_spline_segments;

   public:
    explicit Editor(QWidget* parent);
    ~Editor() override;
    bool eventFilter(QObject* obj, QEvent* event) override;

   private slots:
    void addPoint(const QPointF& pos);
    void updateSpline();
    void handlePointDeleted(PointItem* point);
};
