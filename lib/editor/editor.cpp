#include "editor.hpp"
#include <qlogging.h>

#include <QMouseEvent>

#include "ui_editor.h"

Editor::Editor(QWidget* parent) : QWidget(parent), m_ui(new Ui::Editor) {
    m_ui->setupUi(this);

    m_scene.setSceneRect(-100, -100, 100, 100);
    setup_axes();

    // Обработка кликов
    m_ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    m_ui->graphicsView->viewport()->installEventFilter(this);
    m_ui->graphicsView->setScene(&m_scene);
}

Editor::~Editor() {
    delete m_ui;
}

void Editor::setup_axes() {
    m_scene.addLine(-1000, 0, 1000, 0, QPen(Qt::gray));
    m_scene.addLine(0, -1000, 0, 1000, QPen(Qt::gray));
}

void Editor::addPoint(const QPointF& pos) {
    auto* point = new PointItem(pos.x(), pos.y());
    m_scene.addItem(point);
    m_points.append(point);
    connect(point, &PointItem::positionChanged, this, &Editor::updateSpline);
    connect(point, &PointItem::pointDeleted, this, &Editor::handlePointDeleted);
    updateSpline();
}

void Editor::handlePointDeleted(PointItem* point) {
    m_points.removeOne(point);
    delete point;
    updateSpline();
}

void Editor::updateSpline() {
    // Удаляем старые линии
    qDeleteAll(m_spline_segments);
    m_spline_segments.clear();

    // Рисуем новые линии между точками
    for (int i = 1; i < m_points.size(); ++i) {
        QPointF p1 = m_points[i - 1]->pos();
        QPointF p2 = m_points[i]->pos();
        auto* line =
            m_scene.addLine(p1.x(), p1.y(), p2.x(), p2.y(), QPen(Qt::red, 2));
        m_spline_segments.append(line);
    }
}

bool Editor::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress &&
        obj == m_ui->graphicsView->viewport()) {
        auto* mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() == Qt::LeftButton) {
            auto scene_pos = m_ui->graphicsView->mapToScene(mouse_event->pos());
            qDebug() << "left! " << scene_pos << '\n';
            if (!m_scene.itemAt(scene_pos, QTransform())) {
                addPoint(scene_pos);
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}
