#ifndef QING_DISP_H
#define QING_DISP_H

#include <vector>
#include <cmath>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
using namespace cv;

template<typename T>
inline void qing_vec_min_pos(int& min_pos, vector<T>& in ) {
    T min_val = in[0]; min_pos = 0;
    int len = in.size();
    for(int i = 1; i<len; ++i) { if(in[i]<min_val) {
            min_val = in[i];
            min_pos = i;
        }

    }
}

inline void qing_depth_best_cost(vector<float>& disp, vector<vector<vector<float> > > mcost, const int h, const int w, const int nr_planes ){
    for(int y = 0; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            int d;
           // qx_vec_min_pos(d, mcost[y][x], nr_planes);
            qing_vec_min_pos(d, mcost[y][x]);
            disp[y*w+x] = d;
        }
    }
}


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

//calculate coefficients in [d = ax + by + 1] of a slanted plane, from correspondence input
inline void qing_get_slanted_disparity_w(vector<Point2f>& corners_0, vector<Point2f>& corners_1, float * w ) {
    int size = corners_0.size();
    vector<float> disps(size, 0);
    for(int i = 0; i < size; ++i) {
        disps[i] = corners_0[i].x - corners_1[i].x;
    }

    Mat pts_mat, h_pts_mat, dsp_mat;

    pts_mat = Mat(corners_0).reshape(1);
    h_pts_mat = Mat(size, 3, CV_32FC1, Scalar(1));   pts_mat.copyTo(h_pts_mat(Rect(0, 0, 2, size)));


    dsp_mat = Mat(disps).reshape(1);
    cout << pts_mat.size().width   << '\t' << pts_mat.size().height  << '\t' << pts_mat.type() << endl;     //cout << pts_mat << endl;
    cout << h_pts_mat.size().width << '\t' << h_pts_mat.size().height << '\t' << h_pts_mat.type() << endl;  //cout << h_pts_mat << endl;
    cout << dsp_mat.size().width << '\t' << dsp_mat.size().height  << '\t' << dsp_mat.type() << endl;        //cout << dsp_mat << endl;

    Mat a_mat = h_pts_mat.t() * h_pts_mat;
    Mat b_mat = h_pts_mat.t() * dsp_mat;
    Mat w_mat = Mat(3, 1, CV_32FC1);
    cout << "a_mat: " << a_mat.size() << endl;
    cout << "b_mat: " << b_mat.size() << endl;
    cv::solve(a_mat, b_mat, w_mat, DECOMP_LU);             //singular
    cout << "solve_lu: " << w_mat.t() << endl;
//    cv::solve(a_mat, b_mat, w_mat, DECOMP_NORMAL);       //
//    cout << "solve_normal: " << w_mat << endl;

    memcpy(w, w_mat.ptr<float>(0), sizeof(float)*3);

}

inline float qing_get_interpolate_disp_value(int x, int y, float * w) {
    return w[0] * x + w[1] * y + w[2];
}


#endif // QING_DISP_H
