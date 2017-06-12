/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** lindbscan.cpp
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#include "lindbscan.h"

#include <qmath.h>
#include <QGraphicsScene>
#include <QImage>
#include "clustergraphicsitem.h"
#include "noisegraphicsitem.h"
#include "gridgraphicsitem.h"
#include <QPainter>
#include <cstdlib>

/*!
 * \brief LinDBSCAN::LinDBSCAN
 * \param minPts
 * \param gamma
 * \param points
 * \param parent
 */
LinDBSCAN::LinDBSCAN(int minPts,qreal gamma, QList<Point*>* points, QObject *parent)
{
    this->setParent(parent);

    this->minPts = minPts;
    this->gamma = gamma;
    this->points = points;
    clusters = NULL;
}

/*!
 * \brief LinDBSCAN::~LinDBSCAN
 */
LinDBSCAN::~LinDBSCAN()
{
    if(clusters != NULL)
    {
        QList<Cluster*> values = clusters->values();
        for(int i=0; i < values.size(); i++)
        {
            Cluster* c = values[i];
            delete c;
            c = NULL;
        }

        delete clusters;
        clusters = NULL;
    }
}

/*!
 * \brief LinDBSCAN::getCellFromCoords
 * \param coord
 * \param cells
 * \return
 */
Cell* LinDBSCAN::getCellFromCoords(QVector<int> coord, QHash<QVector<int>, Cell*> *cells)
{
    Cell* result = NULL;

    if(cells->contains(coord))
        result = cells->value(coord);

    return result;
}

/*!
 * \brief LinDBSCAN::createGridTable
 * \param points
 * \param map
 * \param epsilon
 */
void LinDBSCAN::createGridTable(QList<Point*>* points, QHash<QVector<int>, Cell*>* map, qreal epsilon)
{
    for(int i=0; i < points->size(); i++)
    {
        Point* p = points->at(i);

        QVector<int> cellCoords;
        cellCoords.resize(p->coords.size());
        for(int dim=0; dim < cellCoords.size(); dim++)
        {
            qreal temp = p->coords[dim] / epsilon;
            cellCoords[dim] = (int)((temp > 0) ? qFloor(temp) : qCeil(temp));
        }

        Cell* cell = getCellFromCoords(cellCoords, map);
        if(cell == NULL)
        {
            cell = new Cell();
            cell->clusterID = -1;
            cell->Coord = cellCoords;
            cell->epsilon = epsilon;
            cell->color = 0;

            map->insert(cell->Coord, cell);
        }

        // add current point to the cell
        cell->Points.insert(p->id, p);

        p->cell = cellCoords;
    }
}



/*!
 * \brief LinDBSCAN::fill
 * \param pivot
 * \param result
 * \param table
 */
void LinDBSCAN::fill(QVector<int> pivot, Cluster* result, QHash<QVector<int>, Cell*> *table)
{
    QStack<QVector<int> > stack;

    stack.push(pivot);

    while (!stack.isEmpty())
    {
        QVector<int> coord = stack.pop();

        int neighBase = 3;
        Cell* cell = getCellFromCoords(coord, table);
        if(cell != NULL)
        {
            if(cell->clusterID == -1)
            {
                table->remove(coord);

                if (cell->Points.size() >= minPts)
                {
                    cell->clusterID = result->id;

                    for(int k=0; k < cell->Points.size(); k++)
                    {
                        Point* p = cell->Points.values()[k];
                        p->clusterID = result->id;

                        result->Points.insert(p->id, p);
                    }

                    // recurse call for the near cells
                    int nDims = cell->Coord.size();
                    int numOfNeighbours = qPow(neighBase, nDims);

                    // findNeighbors
                    QVector<int> neighbour(nDims);
                    for(int k=0; k<numOfNeighbours; k++)
                    {
                        int offsetSum = 0;
                        for(int dim=0; dim < nDims; dim++)
                        {
                            int div = qPow(neighBase, dim);
                            int offset = ((k / div) % neighBase) - 1;

                            neighbour[dim] = cell->Coord.at(dim) + offset;

                            offsetSum += qAbs(offset);
                        }

                        if(offsetSum != 0)
                        {
                            stack.push(neighbour);
                        }
                    }
                }

                delete cell;
            }
        }
    }
}

/*!
 * \brief LinDBSCAN::floodFill
 * \param pass
 * \param table
 * \return
 */
Cluster* LinDBSCAN::floodFill(int pass, QHash<QVector<int>, Cell*> *table)
{
    Cluster* result = NULL;

    // step 1 scan for entry point cell
    Cell* currentCell = NULL;

    if(table->size() > 0)
        currentCell = *table->begin();

    // step 2 start flood fill from current cell
    if(currentCell != NULL)
    {
        result = new Cluster();
        result->id = pass;

        // start recursive flood fill
        fill(currentCell->Coord, result, table);

        if(result->Points.size() <= 0)
        {
            delete result;
            result = NULL;
        }
    }

    return result;
}

/*!
 * \brief LinDBSCAN::run
 */
void LinDBSCAN::run()
{
    this->doWorkSynch();
}

/*!
 * \brief LinDBSCAN::doWorkSynch
 */
void LinDBSCAN::doWorkSynch()
{
    qDebug() << "Running Lin-DBSCAN...";
    clusters = new QMap<int, Cluster*>();

    QHash<QVector<int>, Cell*> table;
    createGridTable(points, &table, gamma);

    int pass = 0;
    int originalTableSize = table.size();

    while(table.size() > 0)
    {
        Cluster* currentCluster = floodFill(pass, &table);
        if(currentCluster != NULL)
        {
            clusters->insert(currentCluster->id, currentCluster);
        }

        pass++;

        emit clusteringProgress(originalTableSize - table.size(), originalTableSize);
    }

    qDebug() << QString::number(clusters->size()) << " clusters found.";

    emit clusteringCompleted();
}
