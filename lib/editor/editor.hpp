#pragma once

#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <QActionGroup>
#include <QWidget>
#include <utility>
#include "model.hpp"
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
    DataModel* m_data;

    QGraphicsScene m_scene;
    std::vector<std::pair<double, double>> m_spline_points;
    QList<PointItem*> m_points;
    QList<QGraphicsItem*> m_spline_segments;

    void setup_axes();
    void k_updated();

   public:
    explicit Editor(QWidget* parent, DataModel* model);
    ~Editor() override;
    bool eventFilter(QObject* obj, QEvent* event) override;

   private slots:
    void addPoint(const QPointF& pos);
    void updateSpline();
    void handlePointDeleted(PointItem* point);
};
