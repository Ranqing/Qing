#ifndef QING_NORM_H
#define QING_NORM_H

inline double qing_euclidean_dis(cv::Point3f pt1, cv::Point3f pt0)
{
    cv::Point3f pt = pt1 - pt0;
    return sqrt(pt.x * pt.x + pt.y * pt.y + pt.z * pt.z);
}

inline double qing_norm_vec_3(const double * data)
{
    return sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
}


#endif // QING_NORM_H
