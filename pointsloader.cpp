/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** pointsloader.cpp
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#include "pointsloader.h"

#include <QFile>
#include <QDateTime>
#include <QRegExp>
#include <QStringList>
#include <qmath.h>

PointsLoader::PointsLoader(QObject *parent) :
    QObject(parent)
{
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
}

/*!
 * \brief PointsLoader::loadFromFile
 * \param filePath
 * \return
 */
QList<Point*>* PointsLoader::loadFromFile(QString filePath)
{
    QList<Point*>* points = new QList<Point*>();

    // read points from file
    int counter = 0;
    QFile dataset(filePath);
    if(dataset.open(QIODevice::ReadOnly))
    {
        while(!dataset.atEnd())
        {
            QString line(dataset.readLine());

            if(!line.startsWith("#"))
            {
                QStringList fields = line.split(QRegExp("\\s+|\\t+|;|,"), QString::SkipEmptyParts);

                if(fields.size() > 2)
                {
                    Point* p = new Point();
                    p->id = counter;
                    p->clusterID = -1;
                    p->desiredClusterID = "";

                    for(int i=0; i < fields.size() - 1; i++)
                    {
                        bool readOk = false;
                        float temp = fields[i].toFloat(&readOk);

                        if(readOk)
                            p->coords.append(temp);
                    }

                    // assume last field is the cluster id
                    p->desiredClusterID = fields.last();

                    points->append(p);
                }
                else
                {
                    Point* p = new Point();
                    p->id = counter;
                    p->clusterID = -1;
                    p->desiredClusterID = "";

                    for(int i=0; i < fields.size(); i++)
                    {
                        bool readOk = false;
                        float temp = fields[i].toFloat(&readOk);

                        if(readOk)
                            p->coords.append(temp);
                    }
                    points->append(p);
                }

                counter++;
            }
        }

        dataset.close();
    }

    return points;
}
