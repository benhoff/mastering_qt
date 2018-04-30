#ifndef MYPAINTEDITEM_H
#define MYPAINTEDITEM_H

#include <QQuickPaintedItem>
#include <QVector>
#include <QColor>
#include <QPainter>


class ColorPicker: public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit ColorPicker();

    void paint(QPainter *p);
    Q_INVOKABLE int color_to_y(QColor color);
    Q_INVOKABLE QColor y_to_color(int y);

private:

    QColor current_color;
    QVector<QColor> viridis_values;
    QPixmap *pix;
};

#endif // MYPAINTEDITEM_H
