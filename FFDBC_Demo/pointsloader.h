/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** pointsloader.h
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#ifndef POINTSLOADER_H
#define POINTSLOADER_H

#include <QObject>
#include <QList>

#include "common.h"

class PointsLoader : public QObject
{
    Q_OBJECT
public:
    explicit PointsLoader(QObject *parent = 0);

    QList<Point *>* loadFromFile(QString filePath);
    QList<Point*>* makeRandomDataset(int numPoints);

signals:

public slots:

};

#endif // POINTSLOADER_H
