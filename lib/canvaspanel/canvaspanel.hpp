#pragma once
#include <QWidget>
#include <optional>
#include "canvas.hpp"

#include "gradient.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class CanvasPanel;
}
QT_END_NAMESPACE

class CanvasPanel : public QWidget {
    Q_OBJECT
   private:
    Canvas* m_canvas;
    DataModel* m_data;
    Grad m_gradient;
    Ui::CanvasPanel* m_ui;

    std::optional<QColor> ask_color(const QColor& old);

   public:
    explicit CanvasPanel(QWidget* parent, Canvas* cnv, DataModel* model);
    ~CanvasPanel() override;

   private slots:
    void on_top_clicked();
    void on_bottom_clicked();
};
