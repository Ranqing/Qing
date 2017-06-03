#ifndef QING_BASIC_H
#define QING_BASIC_H

#include "qing_common.h"

template<typename T>
//qing_subtract_mat(view_sub_mean_l, m_mat_gray_l, m_mat_mean_l);
void qing_subtract_cv_mat(Mat& sub, const Mat& op1, const Mat& op2) {
    if(op1.size() != op2.size()) { cerr << "different size between op1 and op2!" << endl; return; }
    if(op1.type() != op2.type()) { cerr << "different type between op1 and op2!" << endl; return; }
    sub = Mat::zeros(op1.size(), op1.type());

    T * ptr_1 = (T *)op1.ptr<T>(0);
    T * ptr_2 = (T *)op2.ptr<T>(0);
    T * ptr_sub = (T *)sub.ptr<T>(0);

    int size = op1.size().width * op1.size().height;
    for(int i = 0; i < size; ++i)
        ptr_sub[i] = ptr_1[i] - ptr_2[i];
}

template<typename T>
void qing_sub_vector(vector<T>& sub, vector<T>& op1, vector<T>& op2) {
    if(op1.size() != op2.size()) { cerr << "different size between op1 and op2!" << endl; return; }
    int size = op1.size();
    sub.clear(); sub.resize(size, (T)0);
    for(int i=0; i<size; ++i) {
        sub[i] = op1[i] - op2[i];
    }
 }

template<typename T>
void qing_max_min_vec(vector<T>& vec, T& maxval, T& minval, int* maxpos = 0, int* minpos = 0) {
    int size = vec.size();
    maxval = vec[0];
    minval = vec[0];
    for(int i = 1; i < size; ++i) {
        if(vec[i] > maxval) {
            maxval = vec[i];
            if(maxpos) *maxpos = i;
        }
        if(vec[i] < minval) {
            minval = vec[i];
            if(minpos) *minpos = i;
        }
    }
}



#endif // QING_BASIC_H
