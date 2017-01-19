#ifndef QING_BASIC_H
#define QING_BASIC_H

#include "qing_common.h"

inline double qing_euclidean_dis(cv::Point3f pt1, cv::Point3f pt0)
{
    cv::Point3f pt = pt1 - pt0;
    return sqrt(pt.x * pt.x + pt.y * pt.y + pt.z * pt.z);
}

inline double qing_norm_vec_3(const double * data)
{
    return sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
}

inline double * qing_calc_cam_center(const double * r, const double * t)
{
    double * center = new double[3];

    center[0] = - (r[0] * t[0] + r[3] * t[1] + r[6] * t[2]);
    center[1] = - (r[1] * t[0] + r[4] * t[1] + r[7] * t[2]);
    center[2] = - (r[2] * t[0] + r[5] * t[1] + r[8] * t[2]);

    return center;
}

//inline Point3f qing_calc_cam_center(const Mat& rotation, const Mat& translation)
//{
//    Mat rotation_trans ;
//    cv::transpose(rotation, rotation_trans);
//    Mat center = - rotation_trans * translation;
//    double * pcenter = (double *)center.ptr<double>(0);

//    return Point3f(pcenter[0], pcenter[1], pcenter[2]);
//}

#endif // QING_BASIC_H
