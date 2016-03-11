/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** convexhull.h
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include <QList>
#include <QVector>
#include <QPointF>

class ConvexHull
{
public:
    static QList<QPointF> convex_hull(QList<QPointF> P);

    static qreal cross(const QPointF &O, const QPointF &A, const QPointF &B);
};

#endif // CONVEXHULL_H
