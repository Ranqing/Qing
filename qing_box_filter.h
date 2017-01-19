#ifndef QING_BOX_FILTER_H
#define QING_BOX_FILTER_H

#include <vector>
#include <iostream>
#include <memory>
#include <cstring>
using namespace std;


//cumulative sum along one col
inline void qing_cum_sum_y(const vector<float>& src, const int h, const int w, vector<float>& csum) {

    memset(&csum.front(), 0., sizeof(float) * h * w);

    for(int y = 0; y < h; ++y)
    {
        int cury = y;
        int prey = y ? (y-1) : y;

        for(int x = 0; x < w; ++x)
        {
            int cur_idx = cury * w + x;
            int pre_idx = prey * w + x;
            csum[cur_idx] = src[cur_idx] + csum[pre_idx];
        }
    }
}

//cumulative sum along one row
inline void qing_cum_sum_x(const vector<float> &src, const int h, const int w, vector<float> &csum) {

    memset(&csum.front(), 0., sizeof(float) * h * w);

    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            int cur_idx = y * w + x;
            csum[cur_idx] = x ? (csum[cur_idx - 1] + src[cur_idx]) : src[cur_idx];
        }
    }
}

//o(1) time box filtering using cumulative sum
inline void qing_box_filter(vector<float>& img, const int h, const int w, const int wnd_sz = 3) {

    vector<float> csum(h*w);  memset(&csum.front(), 0.f, sizeof(float)*h*w);
    vector<float> dest(h*w);  memset(&dest.front(), 0.f, sizeof(float)*h*w);

    //cumulative sum over y axis
    qing_cum_sum_y(img, h, w, csum);

    for(int y = 0; y < wnd_sz + 1; ++y) {
        for(int x = 0; x < w; ++x) {
            int idx = y * w + x;
            dest[ idx ] = csum[ idx + wnd_sz * w ];
        }
    }

    for(int y = wnd_sz + 1; y < h - wnd_sz; ++y) {
        for(int x = 0; x < w; ++x) {
            int idx = y * w + x;
            int add_idx = idx + wnd_sz * w;
            int sub_idx = idx - (wnd_sz + 1) * w;

            dest[ idx ] = csum[ add_idx ] - csum[ sub_idx ];
        }
    }

    for(int y = h - wnd_sz; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            int idx = y * w + x;
            int add_idx = (h-1) * w + x;
            int sub_idx = idx - (wnd_sz + 1) * w;

            dest[ idx ] = csum[ add_idx ] - csum[ sub_idx ];
        }
    }

    //cumulative sum over x axis
    qing_cum_sum_x(dest, h, w, csum);
    for(int y = 0; y < h; ++y) {
        for(int x = 0; x < wnd_sz+1; ++x) {
            int idx = y * w + x;
            dest[ idx ] = csum[ idx + wnd_sz ];
        }
        for(int x = wnd_sz + 1; x < w - wnd_sz; ++x) {
            int idx = y * w + x;
            int add_idx = idx + wnd_sz;
            int sub_idx = idx - (wnd_sz + 1);
            dest[ idx ] = csum[ add_idx ] - csum [ sub_idx ];
        }
        for(int x = w - wnd_sz; x < w; ++x) {
            int idx = y * w + x;
            int add_idx = y * w + w - 1;
            int sub_idx = y * w - (wnd_sz + 1);
            dest[ idx ] = csum[ add_idx ] - csum[ sub_idx ];
        }
    }

    copy( dest.begin(), dest.end(), img.begin() );
}
#endif // QING_BOX_FILTER_H
