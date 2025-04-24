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
        emit pallete_change(m_a, m_b);
        update();
    }
    void set_b_color(const QColor& col) {
        m_b = col;
        emit pallete_change(m_a, m_b);
        update();
    }

    QColor a() const { return m_a; }
    QColor b() const { return m_b; }
   signals:
    void pallete_change(const QColor& a, const QColor& b);

   protected:
    void paintEvent(QPaintEvent* event) override;

   private:
    QColor m_a = Qt::red;
    QColor m_b = Qt::blue;
};
