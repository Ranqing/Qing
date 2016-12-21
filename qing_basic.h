#ifndef QING_BASIC_H
#define QING_BASIC_H

#include "qing_common.h"

inline double qing_euclidean_dis(Point3f pt1, Point3f pt0)
{
    Point3f pt = pt1 - pt0;
    return sqrt(pt.x * pt.x + pt.y * pt.y + pt.z * pt.z);
}

#endif // QING_BASIC_H
