/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** lindbscan.h
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#ifndef LINDBSCAN_H
#define LINDBSCAN_H

#include "common.h"

class LinDBSCAN
{
public:
    LinDBSCAN(int minPts, qreal gamma, QList<Point*>* points);
    ~LinDBSCAN();

    void doWorkSynch();
    void run();

    qreal getGamma() { return gamma; }
    QList<Cluster*> getClusters() { return (clusters)? clusters->values() : QList<Cluster*>(); }

protected:
    qreal gamma;
    int minPts;
    QList<Point*>* points;
    QMap<int, Cluster*>* clusters;

    void createGridTable(QList<Point*>* points, QHash<QVector<int>, Cell*>* map, qreal gamma);
    Cluster* floodFill(int pass, QHash<QVector<int>, Cell*> *table);
    void fill(QVector<int> pivot, Cluster* result, QHash<QVector<int>, Cell*> *table);
    Cell* getCellFromCoords(QVector<int> coord, QHash<QVector<int>, Cell*> *cells);
};

#endif // LINDBSCAN_H
