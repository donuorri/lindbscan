/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** clusterraphicsitem.h
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#ifndef CLUSTERGRAPHICSITEM_H
#define CLUSTERGRAPHICSITEM_H

#include "common.h"

#include <QGraphicsItem>

class ClusterGraphicsItem : public QGraphicsItem
{
public:
    ClusterGraphicsItem(Cluster* cluster, QColor color, float gamma, float penSize, bool drawConvexHull = true, QGraphicsItem *parent = 0);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
    QRectF boundingRect() const;

protected:
    QColor color;
    float gamma;
    float penSize;
    QList<QPointF> points;
    QList<QPointF> cells;
    QList<QPointF> hull;
    QPointF centroid;
    QSizeF size;
    bool drawConvexHull;
};

#endif // CLUSTERGRAPHICSITEM_H
