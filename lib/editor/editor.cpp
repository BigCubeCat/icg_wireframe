#include "editor.hpp"
#include <qlogging.h>
#include <qnamespace.h>

#include <QMouseEvent>
#include <vector>

#include "ui_editor.h"

const int kViewSize = 100;
const float kScl = 1.1;

const QPen kSplinePen = QPen(Qt::red, 3);
const QPen kAxisPen = QPen(Qt::black);
const QPen kDotPen = QPen(Qt::green, 2);

Editor::Editor(QWidget* parent, DataModel* model)
    : QWidget(parent), m_ui(new Ui::Editor), m_view(this), m_data(model) {
    m_ui->setupUi(this);
    m_ui->layout->addWidget(&m_view);

    m_scene.setSceneRect(-kScl * kViewSize, -kScl * kViewSize,
                         kScl * 2 * kViewSize, kScl * 2 * kViewSize);
    setup_axes();

    // Обработка кликов
    m_view.setRenderHint(QPainter::Antialiasing);
    m_view.viewport()->installEventFilter(this);
    m_view.setScene(&m_scene);

    connect(m_ui->mSpinBox, &QSpinBox::valueChanged, m_data, &DataModel::set_m);
    connect(m_ui->nSpinBox, &QSpinBox::valueChanged, m_data, &DataModel::set_n);
    connect(m_ui->m1SpinBox, &QSpinBox::valueChanged, m_data,
            &DataModel::set_m1);

    connect(m_data, &DataModel::redraw_spline, this, &Editor::updateSpline);

    connect(m_ui->applyButton, &QPushButton::clicked, this, &Editor::apply);
    connect(m_ui->normalizeButton, &QPushButton::clicked, this,
            &Editor::normalize);
}

Editor::~Editor() {
    delete m_ui;
}

void Editor::setup_axes() {
    m_scene.addLine(-1000, 0, 1000, 0, kAxisPen);
    m_scene.addLine(0, -1000, 0, 1000, kAxisPen);
    m_scene.addLine(kViewSize, -kViewSize, kViewSize, kViewSize, kAxisPen);
    m_scene.addLine(-kViewSize, -kViewSize, -kViewSize, kViewSize, kAxisPen);
    m_scene.addLine(-kViewSize, kViewSize, kViewSize, kViewSize, kAxisPen);
    m_scene.addLine(-kViewSize, -kViewSize, kViewSize, -kViewSize, kAxisPen);
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
    updateSpline();
    k_updated();
}

void Editor::updateSpline() {
    // Удаляем старые линии
    qDeleteAll(m_spline_segments);
    qDeleteAll(m_spline);
    m_spline_segments.clear();
    m_spline.clear();
    const auto k = m_points.size();
    // Рисуем новые линии между точками
    for (int i = 1; i < k; ++i) {
        QPointF p1 = m_points[i - 1]->pos();
        QPointF p2 = m_points[i]->pos();
        auto* line = m_scene.addLine(p1.x(), p1.y(), p2.x(), p2.y(), kDotPen);
        m_spline_segments.append(line);
    }
    const auto spline_points = m_data->spline();
    const auto k1 = static_cast<int>(spline_points.size());
    for (int i = 1; i < k1; ++i) {
        auto p1 = spline_points[i - 1];
        auto p2 = spline_points[i];
        auto* line =
            m_scene.addLine(p1.x() * kViewSize, p1.y() * kViewSize,
                            p2.x() * kViewSize, p2.y() * kViewSize, kSplinePen);
        m_spline.append(line);
    }
}

bool Editor::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress && obj == m_view.viewport()) {
        auto* mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() == Qt::LeftButton) {
            auto scene_pos = m_view.mapToScene(mouse_event->pos());
            if (!m_scene.itemAt(scene_pos, QTransform())) {
                addPoint(scene_pos);
            }
        }
    }
    k_updated();
    return QWidget::eventFilter(obj, event);
}

void Editor::k_updated() {
    auto k = m_points.size();
    m_ui->kSpinBox->setValue(k);
    m_ui->applyButton->setEnabled(k >= 4);
}

void Editor::apply() {
    const auto k = m_points.size();
    std::vector<double> u(k);
    std::vector<double> v(k);
    for (int i = 0; i < k; ++i) {
        QPointF point = m_points[i]->pos();
        u[i] = point.x() / kViewSize;
        v[i] = point.y() / kViewSize;
    }
    m_data->set_points(std::move(u), std::move(v));
}

void Editor::normalize() {
    int maximum_x = -2 * kViewSize;
    int minimum_x = 2 * kViewSize;
    int maximum_y = -2 * kViewSize;
    int minimum_y = 2 * kViewSize;
    const auto k = m_points.size();
    for (int i = 0; i < k; ++i) {
        QPointF point = m_points[i]->pos();
        auto x = static_cast<int>(point.x());
        auto y = static_cast<int>(point.y());
        minimum_x = std::min(minimum_x, x);
        minimum_y = std::min(minimum_y, y);
        maximum_x = std::max(maximum_x, x);
        maximum_y = std::max(maximum_y, y);
    }
    const auto width = (maximum_x - minimum_x) / 2;
    const auto height = (maximum_y - minimum_y) / 2;
    const auto size = std::max(width, height);
    for (int i = 0; i < k; ++i) {
        m_points[i]->setX(((m_points[i]->x() - minimum_x) / size * kViewSize) -
                          kViewSize);
        m_points[i]->setY(((m_points[i]->y() - minimum_y) / size * kViewSize) -
                          kViewSize);
    }
    updateSpline();
}
