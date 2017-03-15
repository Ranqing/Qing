#ifndef QING_DISP_H
#define QING_DISP_H

#include <vector>
#include <cmath>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
using namespace cv;

//离散级数到视差值的换算, d(k) = (dmin*(1-k) + dmax*(k)) / (dmax-dmin)
inline float qing_k_2_disp(const int maxd, const int mind, const int k) {
    return (((mind)*(1-(k)) + (maxd)*(k))/((maxd)- (mind)));
}

//视差值换算到离散级数
inline int qing_disp_2_k(const int maxd, const int mind, const float d) {
    return ((d) - (mind)/((maxd)-(mind))) ;
}

// qing_disp_2_depth(&points.front(), &colors.front(), ptr_disp, ptr_msk, ptr_bgr, ptr_q_matrix, m_crop_point_l);
inline float qing_disp_2_depth(vector<Vec3f>& points, vector<Vec3f>& colors,  float * ptr_dsp, unsigned char * ptr_msk, unsigned char * ptr_bgr, double * qmtx, Point2i crop_point, int w, int h) {

    for(int y = 0, idx = 0; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            if(0==ptr_msk[idx] || 0 == ptr_dsp[idx]) {idx++;continue;}
            else {
                double uvd1[4], xyzw[4] ;
                uvd1[0] = x + crop_point.x;
                uvd1[1] = y + crop_point.y;
                uvd1[2] = ptr_dsp[idx];
                uvd1[3] = 1.0;

                xyzw[0] = qmtx[ 0] * uvd1[0] + qmtx[ 1] * uvd1[1] + qmtx[ 2] * uvd1[2] + qmtx[ 3] * uvd1[3];
                xyzw[1] = qmtx[ 4] * uvd1[0] + qmtx[ 5] * uvd1[1] + qmtx[ 6] * uvd1[2] + qmtx[ 7] * uvd1[3];
                xyzw[2] = qmtx[ 8] * uvd1[0] + qmtx[ 9] * uvd1[1] + qmtx[10] * uvd1[2] + qmtx[11] * uvd1[3];
                xyzw[3] = qmtx[12] * uvd1[0] + qmtx[13] * uvd1[1] + qmtx[14] * uvd1[2] + qmtx[15] * uvd1[3];

                points.push_back(Vec3f(xyzw[0]/xyzw[3], xyzw[1]/xyzw[3], xyzw[2]/xyzw[3]));
                colors.push_back(Vec3f(ptr_bgr[3*idx + 2], ptr_bgr[3*idx + 1], ptr_bgr[3*idx + 0]));
                idx++;
            }
        }
    }
    cout << points.size() << " points are generated... " << endl;

}

#endif // QING_DISP_H
