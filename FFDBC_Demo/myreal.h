/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** myreal.h
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#ifndef MYREAL_H
#define MYREAL_H

#include <QtCore>

class MyQReal
{
public:
    MyQReal(qreal v = 0) { value = v; }

    qreal value;
};

inline bool operator==(const MyQReal &v1, const MyQReal &v2) { return (v1.value == v2.value); }

inline uint qHash(const MyQReal &key, uint seed = 0)
{
    return ((unsigned)key.value) ^ seed;
}

#endif // MYREAL_H
