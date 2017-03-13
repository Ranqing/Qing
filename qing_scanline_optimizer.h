#ifndef QING_SCANLINE_OPTIMIZER_H
#define QING_SCANLINE_OPTIMIZER_H

#include "qing_matching_cost.h"

#define QING_SO_TAU 15
#define QING_SO_P1 1.0
#define QING_SO_P2 3.0

inline void qing_add_so_mcost(float * out_cost_vol, float * temp_cost_vol, int d_range, int h, int w) {
    int image_size = h * w;
    for(int d = 0; d < d_range; ++d) {
        float * out = out_cost_vol + d * image_size;
        float * temp = temp_cost_vol + d * image_size;
        for(int y = 0, idx = 0; y < w; ++y) {
            for(int x = 0; x < h; ++x) {
                out[idx] += temp[idx];
                idx++;
            }
        }
    }
}

inline void qing_so_optim_params(int x1, int x2, int y1, int y2, int d, float& P1, float& P2) {

}

inline void qing_optim(float * so_cost_vol, float * in_cost_vol, int d_range, int h, int w, int x1, int x2, int y1, int y2) {

    int image_size = h * w;
    int idx1 = y1 * w + x1;         //current  pixel
    int idx2 = y2 * w + x2;         //previous pixel

    //i ~ minL_r(p_r,i)
    float * ptr_mcost = so_cost_vol;
    float min_so_mcost = ptr_mcost[idx2];
    for(int i = 1; i < d_range; ++i) {
        ptr_mcost += image_size;
        if(ptr_mcost[idx2] < min_so_mcost) min_so_mcost = ptr_mcost[idx2];
    }

    float min_k_cr = min_so_mcost;
    float so_mcost ;
    float * ptr_so_mcost, * ptr_sub_so_mcost, * ptr_add_so_mcost;
    for(int d = 0; d < d_range; ++d) {
        ptr_mcost = in_cost_vol + d * image_size;
        ptr_so_mcost = so_cost_vol + d * image_size;
        ptr_sub_so_mcost = ptr_so_mcost - image_size;
        ptr_add_so_mcost = ptr_so_mcost + image_size;

        so_mcost = ptr_mcost[idx1] - min_k_cr;

        float P1, P2;
        qing_so_optim_params(x1,x2,y1,y2,d,P1,P2) ;

        //min(cr(p-r,d), min_k(cr(p-r,k)+P2)
        min_so_mcost = min_k_cr + P2;
        if(min_so_mcost > ptr_so_mcost[idx2]) {
            min_so_mcost = ptr_so_mcost[idx2];
        }

        if(d != 0 && min_so_mcost > (ptr_sub_so_mcost[idx2] + P1) ) {
            min_so_mcost = ptr_sub_so_mcost[idx2] + P1;
        }

        if(d != 0 && min_so_mcost > (ptr_add_so_mcost[idx2] + P1) ) {
            min_so_mcost = ptr_add_so_mcost[idx2] + P1;
        }

        ptr_so_mcost[idx1] = so_mcost + min_so_mcost;
    }
}

inline void qing_scanline_optimize_y_sub(float * out_cost_vol, float * in_cost_vol, int d_range, int h, int w, int y1, int y2) {

    for(int x = 0; x < w; ++x) {
        qing_optim(out_cost_vol, in_cost_vol, h, w, d_range, x, x, y1, y2 );
    }
}

inline void qing_scanline_optimize_x_sub(float * out_cost_vol, float * in_cost_vol, int d_range, int h, int w, int x1, int x2) {
    for(int y = 0; y < h; ++y) {
        qing_optim(out_cost_vol, in_cost_vol, h, w, d_range, x1, x2, y, y);
    }
}

inline void qing_scanline_optimize_y(float * out_cost_vol, float * in_cost_vol, int d_range, int h, int w, int yc, int r) {

    cout << "scanline optimize vertical..\tr = " << r << endl;

    int image_size = h * w;
    int total_size = d_range * image_size;

    float * temp_cost_vol = new float[total_size];
    if(0==temp_cost_vol) {
        cerr << "bad alloc..." << endl;
        return ;
    }
    for(int d = 0; d < d_range; ++d) {
        int idx_d = d * image_size + yc * w;
        float * ptr_yc_in = in_cost_vol + idx_d;
        float * ptr_yc_temp = temp_cost_vol + idx_d;
        memcpy(ptr_yc_temp, ptr_yc_in, sizeof(float)*w);
    }

    for(int y = yc+r; 0<=y && h>=y; y+=r) {
        qing_scanline_optimize_y_sub(temp_cost_vol, in_cost_vol, d_range, h, w, y, y-r);
    }

    qing_add_so_mcost(out_cost_vol, temp_cost_vol, d_range, h, w);
    if(0!=temp_cost_vol) {
        delete[] temp_cost_vol; temp_cost_vol = 0;
    }

}

inline void qing_scanline_optimize_x(float * out_cost_vol, float * in_cost_vol, int d_range, int h, int w, int xc, int r) {

    cout << "scanline optimize horizontal..\tr=" << r << endl;

    int image_size = h * w;
    int total_size = d_range * image_size;

    float * temp_cost_vol = new float[total_size];
    if(0==temp_cost_vol) {
        cerr << "bad alloc..." << endl;
        return ;
    }

    for(int d = 0; d < d_range; ++d) {
        for(int y = 0; y < h; ++y) {
             int idx = d * image_size + y * w + xc;
             temp_cost_vol[idx] = in_cost_vol[idx];
        }
    }

    for(int x = xc+r; 0<=x && w>=x; x+=r) {
        qing_scanline_optimize_x_sub(temp_cost_vol, in_cost_vol,d_range, h, w, x, x-r);
    }

    qing_add_so_mcost(out_cost_vol, temp_cost_vol, d_range, h, w);
    if(0!=temp_cost_vol) {
        delete[] temp_cost_vol; temp_cost_vol = 0;
    }
}


#endif // QING_SCANLINE_OPTIMIZER_H
