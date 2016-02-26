/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** noisegraphicsitem.h
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#ifndef NOISEGRAPHICSITEM_H
#define NOISEGRAPHICSITEM_H

#include "common.h"

#include <QGraphicsItem>

class NoiseGraphicsItem : public QGraphicsItem
{
public:
    NoiseGraphicsItem(QList<Point*> points, QColor noiseColor = Qt::white, float penSize = 3, QGraphicsItem* parent = 0);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
    QRectF boundingRect() const { return boundingRectangle; }

protected:
    QList<QPointF> points;
    QRectF boundingRectangle;

    QColor noiseColor;
    float penSize;
};

#endif // NOISEGRAPHICSITEM_H
