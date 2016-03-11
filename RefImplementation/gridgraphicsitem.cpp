/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** gridgraphicsitem.cpp
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#include "gridgraphicsitem.h"

#include <QPainter>

/*!
 * \brief GridGraphicsItem::GridGraphicsItem
 * \param eps
 * \param sceneRectangle
 * \param parent
 */
GridGraphicsItem::GridGraphicsItem(float gamma, QRectF sceneRectangle, QGraphicsItem *)
{
    _Gamma = gamma;
    _SceneRectangle = sceneRectangle;
}

/*!
 * \brief GridGraphicsItem::paint
 * \param painter
 * \param option
 * \param widget
 */
void GridGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPen gridPen(Qt::darkGray);
    gridPen.setCosmetic(true);
    gridPen.setStyle(Qt::DashLine);

    // disable grid rendering for more than 10K lines
    if((_SceneRectangle.width() / _Gamma) <= 10000)
    {
        painter->setPen(gridPen);
        for(float k = _SceneRectangle.left(); k <= _SceneRectangle.right(); k += _Gamma)
        {
            QLineF line(k, _SceneRectangle.top(), k, _SceneRectangle.bottom());
            painter->drawLine(line);
        }
        for(float k = _SceneRectangle.top(); k <= _SceneRectangle.bottom(); k += _Gamma)
        {
            QLineF line(_SceneRectangle.left(), k, _SceneRectangle.right(), k);
            painter->drawLine(line);
        }
    }
}
