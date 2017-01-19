#ifndef QING_MEDIAN_FILTER_H
#define QING_MEDIAN_FILTER_H

#include <vector>
#include <iostream>
using namespace std;
typedef unsigned char uchar;

inline void qing_median_filter(vector<int>& out, const vector<int>& in, const vector<uchar>& mask, const int h, const int w, const int wnd_size, const int BIN_SIZE) {

    int sz = in.size(), offset = wnd_size * 0.5;
    out.resize(sz, 0.f); copy(in.begin(), in.end(), out.begin());

    for(int y = 0, idx = 0; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            if(0==mask[idx]) {idx++;continue;}

            vector<int> histogram(BIN_SIZE, 0);
            int total = 0;
            for(int dy = -offset; dy <= offset; ++dy) {
                int cur_y = y + dy;
                if(0>cur_y || h <= cur_y) continue;
                for(int dx = -offset; dx <= offset; ++dx ) {
                    int cur_x = x + dx;
                    if(0>cur_x || w <= cur_x) continue;
                    int cur_idx = cur_y * w + cur_x;

                    if(0==mask[cur_idx]) continue;

                    int k = in[cur_idx];

                    histogram[k] ++;
                    total++;
                }
            }

            int cnt = 0, median_bin = 0;
            for(int bin = 0; bin < BIN_SIZE; ++bin) {
                cnt += histogram[bin];
                if( 2*cnt >= total ) {
                    median_bin = bin; break;
                }
            }

            out[idx++] = median_bin;
        }
    }

    cout << "\tqing_median_filter done..." << endl;
}

#endif // QING_MEDIAN_FILTER_H
