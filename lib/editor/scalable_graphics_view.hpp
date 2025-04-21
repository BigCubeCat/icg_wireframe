#pragma once

#include <QGraphicsView>

class ScalableGraphicsView : public QGraphicsView {
    Q_OBJECT
   public:
    explicit ScalableGraphicsView(QWidget* parent = nullptr);

   protected:
    void wheelEvent(QWheelEvent* event) override;
};
