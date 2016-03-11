/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** gridgraphicsitem.h
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#ifndef GRIDGRAPHICSITEM_H
#define GRIDGRAPHICSITEM_H

#include <QGraphicsItem>

class GridGraphicsItem : public QGraphicsItem
{
public:
    GridGraphicsItem(float gamma, QRectF sceneRectangle, QGraphicsItem *parent = 0);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
    QRectF boundingRect() const { return _SceneRectangle; }

private:
    float _Gamma;
    QRectF _SceneRectangle;
};

#endif // GRIDGRAPHICSITEM_H
