#ifndef COLOR_VIEW_H
#define COLOR_VIEW_H

#include <QColor>
#include <QWidget>

class color_view : public QWidget {
    Q_OBJECT
public:
    explicit color_view(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event);
    void set_color(const QColor& color);
    QColor get_color();

private:
    QColor w_color = QColor(255, 255, 255);
};

#endif // COLOR_VIEW_H
