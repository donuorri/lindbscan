/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** convexhull.cpp
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#include "convexhull.h"

bool pointComparer(const QPointF &p1, const QPointF& p2) {
    return p1.x() < p2.x() || (p1.x() == p2.x() && p1.y() < p2.y());
}

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
qreal ConvexHull::cross(const QPointF &O, const QPointF &A, const QPointF &B)
{
    return ((A.x() - O.x()) * (B.y() - O.y()) - (A.y() - O.y()) * (B.x() - O.x()));
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
QList<QPointF> ConvexHull::convex_hull(QList<QPointF> P)
{
    int n = P.size(), k = 0;
    QVector<QPointF> H(2*n);

    // Sort points lexicographically
    qSort(P.begin(), P.end(), pointComparer);

    // Build lower hull
    for (int i = 0; i < n; i++) {
            while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
            H[k++] = P[i];
    }

    // Build upper hull
    for (int i = n-2, t = k+1; i >= 0; i--) {
            while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
            H[k++] = P[i];
    }

    QList<QPointF> result;
    result.reserve(k);
    for(int i=0; i < k; i++)
    {
        result.append(H[i]);
    }
    //H.resize(k);

    return result;
}
