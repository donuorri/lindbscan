/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** noisegraphicsitem.cpp
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#include "noisegraphicsitem.h"

#include <QPainter>

NoiseGraphicsItem::NoiseGraphicsItem(QList<Point*> points, QColor noiseColor, float penSize, QGraphicsItem* parent) :
    QGraphicsItem(parent)
{
    float minX = 0, minY = 0, maxX = 0, maxY = 0;

    for(int i=0; i < points.size(); i++)
    {
        Point* p = points.at(i);

        this->points.append(QPointF(p->coords[0], p->coords[1]));

        if(p->coords[0] < minX) minX = p->coords[0];
        if(p->coords[0] > maxX) maxX = p->coords[0];
        if(p->coords[1] < minY) minY = p->coords[1];
        if(p->coords[1] > maxY) maxY = p->coords[1];
    }

    boundingRectangle = QRectF(minX, minY, (maxX - minX), (maxY - minY));

    this->noiseColor = noiseColor;
    this->penSize = penSize;
}

/*!
 * \brief NoiseGraphicsItem::paint
 * \param painter
 * \param option
 * \param widget
 */
void NoiseGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPen pen(noiseColor);
    pen.setCosmetic(true);
    painter->setRenderHint(QPainter::Antialiasing, false);

    pen.setWidth(penSize);
    painter->setPen(pen);
    QTransform tr = painter->transform();
    painter->resetTransform();
    for(int i=0; i < points.size(); i++)
    {
        painter->drawPoint(points.at(i) * tr);
    }
    painter->setTransform(tr);
}
