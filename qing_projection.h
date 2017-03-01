#ifndef QING_PROJECTION_H
#define QING_PROJECTION_H

#include "qing_common.h"

inline double * qing_calc_cam_center_from_rt(const double * r, const double * t)
{
    double * center = new double[3];

    center[0] = - (r[0] * t[0] + r[3] * t[1] + r[6] * t[2]);
    center[1] = - (r[1] * t[0] + r[4] * t[1] + r[7] * t[2]);
    center[2] = - (r[2] * t[0] + r[5] * t[1] + r[8] * t[2]);

    return center;
}

inline void qing_calc_cam_center_from_rt(Mat& cam, Mat& R, Mat& T) {
    double * rdata  = (double *)R.ptr<double>(0);
    double * tdata  = (double *)T.ptr<double>(0);
    double * cdata = qing_calc_cam_center_from_rt(rdata, tdata);              //center in qing_calc_cam_center must be create by 'new', or data in this memory will be freshed by 'create' then

    cam.create(3,1,CV_64FC1);
    memcpy(cam.data, cdata, sizeof(double)*3);
}

inline void qing_calc_pmatrix_from_rt(Mat& Pmat, Mat& R, Mat& T)
{
    double p[12];
    double * rdata  = (double *)R.ptr<double>(0);
    double * tdata  = (double *)T.ptr<double>(0);

    p[0] = rdata[0] ;   p[1] = rdata[1] ;   p[2] = rdata[2] ;  p[3] = tdata[0];
    p[4] = rdata[3] ;   p[5] = rdata[4] ;   p[6] = rdata[5] ;  p[7] = tdata[1];
    p[8] = rdata[6] ;   p[9] = rdata[7] ;   p[10] = rdata[8] ; p[11] = tdata[2];

    Pmat.create(3, 4, CV_64FC1);
    memcpy(Pmat.data, p, sizeof(double)*12);
}

inline Mat qing_cam_coord_to_world_coord(const Mat& rotation, const Mat& translation, const Mat& c_coord)
{
    Mat w_coord = rotation.t() * (c_coord - translation);
    return w_coord;
}

//inline Point3f qing_calc_cam_center(const Mat& rotation, const Mat& translation)
//{
//    Mat rotation_trans ;
//    cv::transpose(rotation, rotation_trans);
//    Mat center = - rotation_trans * translation;
//    double * pcenter = (double *)center.ptr<double>(0);

//    return Point3f(pcenter[0], pcenter[1], pcenter[2]);
//}



#endif // QING_PROJECTION_H
