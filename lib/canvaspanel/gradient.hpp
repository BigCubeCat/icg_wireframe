#pragma once

#include <qcolor.h>
#include <QEvent>
#include <QWidget>

class Grad : public QWidget {
    Q_OBJECT

   public:
    explicit Grad(QWidget* parent = nullptr) : QWidget(parent) {}

    void set_a_color(const QColor& col) {
        m_a = col;
        update();
    }
    void set_b_color(const QColor& col) {
        m_b = col;
        update();
    }

    QColor a() const { return m_a; }
    QColor b() const { return m_b; }

   protected:
    void paintEvent(QPaintEvent* event) override;

   private:
    QColor m_a = Qt::red;
    QColor m_b = Qt::blue;
};
