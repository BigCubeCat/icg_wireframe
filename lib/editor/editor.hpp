#pragma once

#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <QActionGroup>
#include <QWidget>
#include "model.hpp"
#include "point_item.hpp"
#include "scalable_graphics_view.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class Editor;
}
QT_END_NAMESPACE

class Editor : public QWidget {
    Q_OBJECT
   private:
    Ui::Editor* m_ui;
    ScalableGraphicsView m_view;
    DataModel* m_data;

    QGraphicsScene m_scene;
    QList<PointItem*> m_points;
    QList<QGraphicsItem*> m_spline_segments;
    QList<QGraphicsItem*> m_spline;

    void setup_axes();
    void k_updated();

   public:
    explicit Editor(QWidget* parent, DataModel* model);
    ~Editor() override;
    bool eventFilter(QObject* obj, QEvent* event) override;

    void open_spline();

   private slots:
    void addPoint(const QPointF& pos, bool update_spline = false);
    void updateSpline();
    void handlePointDeleted(PointItem* point);
    void apply();
    void normalize();
};
