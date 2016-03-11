/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** common.h
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#ifndef COMMON_H
#define COMMON_H

#include <QList>
#include <QMap>
#include <QHash>
#include <QVector>
#include <QPoint>
#include <QString>

#include "myreal.h"

class Point
{
public:
    int id;

    QVector<float> coords;
    QVector<int> cell;

    int clusterID;
    QString desiredClusterID;
};

class Cell
{
public:
    int clusterID;
    qreal epsilon;
    qreal color;
    int level;

    QVector<int> Coord;
    QHash<int, Point*> Points;
};

class Cluster
{
public:
    int id;
    qreal epsilon;
    QList<Point*> Points;
};

inline uint qHash(const QPoint &key, uint seed)
{
    return ((key.x() << 16) + key.y()) ^ seed;
}

inline uint qHash(const QVector<int> &key, uint seed)
{
    uint result = 0;

    quint8 shift = 0;
    for(int i=0; i < key.size(); i++)
    {
        result += (key[i] << shift);

        shift += 4;
        if(shift > 24) shift = 0;
    }

    return result ^ seed;
}

#endif // COMMON_H
