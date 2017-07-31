#ifndef QING_GUIDED_FILTER_H
#define QING_GUIDED_FILTER_H

#include "qing_box_filter.h"

template <typename T>
inline void qing_guided_filter_1ch(const vector<T>& guidance, const int h, const int w, vector<float>& img, const int wnd_sz = 9, const float eps = 0.0001) {

//    vector<float> weights(h*w);
//    memset(&weights.front(), 1, sizeof(float)*h*w);
//    qing_box_filter(weights, h, w, wnd_sz);

    //to-do: implementation



}

#endif // QING_GUIDED_FILTER_H
