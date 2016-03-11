/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** clustergraphicsitem.cpp
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#include "clustergraphicsitem.h"
#include "convexhull.h"

#include <QPainter>

ClusterGraphicsItem::ClusterGraphicsItem(Cluster *cluster, QColor color, float gamma, float penSize, bool convexHull, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    this->gamma = gamma;
    this->color = color;
    this->penSize = penSize;

    float centX = 0, centY = 0;

    // draw an ellipse for each cluster
    for(int i=0; i < cluster->Points.size(); i++)
    {
        Point* p = cluster->Points[i];
        points.append(QPointF(p->coords[0], p->coords[1]));

        centX += p->coords[0];
        centY += p->coords[1];
    }
    centX /= cluster->Points.size();
    centY /= cluster->Points.size();

    float maxWidth = 0, maxHeight = 0;
    for(int i=0; i < cluster->Points.size(); i++)
    {
        Point* p = cluster->Points[i];

        float distX = qAbs(p->coords[0] - centX);
        float distY = qAbs(p->coords[1] - centY);

        if(distX > maxWidth) maxWidth = distX;
        if(distY > maxHeight) maxHeight = distY;
    }

    centroid.setX(centX);
    centroid.setY(centY);

    size.setWidth(maxWidth);
    size.setHeight(maxHeight);

    drawConvexHull = convexHull;
    hull = ConvexHull::convex_hull(points);
}

/*!
 * \brief ClusterGraphicsItem::paint
 * \param painter
 * \param option
 * \param widget
 */
void ClusterGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPen pen(color);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing, false);

    // convex hull
    if (drawConvexHull)
    {
        QPolygonF polygon(hull.toVector());

        QColor fillColor(color.red(), color.green(), color.blue(), 64);
        QBrush fillBrush(fillColor);
        painter->setBrush(fillBrush);
        painter->drawPolygon(polygon);
    }

    // points
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

/*!
 * \brief ClusterGraphicsItem::boundingRect
 * \return
 */
QRectF ClusterGraphicsItem::boundingRect() const
{
    return QRectF(centroid.x() - (size.width() / 2.0f),
                  centroid.y() - (size.height() / 2.0f),
                  size.width(),
                  size.height());
}
