#ifndef QING_BILATERAL_FILTER_H
#define QING_BILATERAL_FILTER_H

#include "qing_macros.h"
#include "qing_matching_cost.h"

inline float * qing_get_range_weighted_table(float sigma_range, int len) {
    float * table_range, * range_table_x;
    table_range = new float[len];
    range_table_x = &table_range[0];
    for(int y = 0; y<len; ++y) {
        *range_table_x++ = exp(-double(y*y)/(2*sigma_range*sigma_range));
    }
#if 0
    cout << "sigma_range = " << sigma_range << endl;
    cout << "len = " << len << endl;
    for(int y = 0; y < len; ++y) {
        cout << table_range[y] << ' ';
    }
    cout << endl;
#endif
    return table_range;
}

inline float * qing_get_spatial_weighted_table(float sigma_spatial, int len) {
    float * table_spatial, * spatial_table_x;
    table_spatial = new float[len];
    spatial_table_x = &table_spatial[0];
    for(int y = 0; y < len; ++y) {
        *spatial_table_x++ = exp(-double(y*y)/(2*sigma_spatial*sigma_spatial));
    }
#if 0
    cout << "sigma_spatial = " << sigma_spatial << endl;
    cout << len << endl;
    for(int y = 0; y < len; ++y) {
        cout << table_spatial[y] << ' ';
    }
    cout << endl;
#endif
    return table_spatial;
}

//the max direction param = 1, because of the smoothness
inline float * qing_get_directions(float step, int len) {
    float * table_direction, * direction_table_x;
    table_direction = new float[len];
    direction_table_x = &table_direction[0];

    int offset = len * 0.5;
    for(int i = -offset; i <= offset; ++i) {
        *direction_table_x++ =  i * step;
    }
# if 1
    for(int ii = 0; ii < len; ++ii)
        cout << table_direction[ii] << ' ';
    cout << endl;
# endif
    return table_direction;
}

inline void  qing_approximated_gray_bilateral_filter(float * out, float * in, unsigned char * gray, const int w, const int h, const int wnd, float * range_table, float * spatial_table) {

    int image_size = h * w;
    int offset = wnd * 0.5;

    float * temp = new float[image_size];
    float * in_ = in;                       /*horizontal filtering*/
    float * out_ = temp;

    for(int y = 0; y < h; ++y) {
        int idy = y * w;
        for(int x = 0; x < w; ++x) {

            int idx = idy + x;
            unsigned char gray_c = *(gray + idx);

            double sum = 0.0, sum_div = 0.0;

            for(int k = -offset; k <= offset; ++k) {
                int xk = k+x;
                xk = max(0,  xk);
                xk = min(w-1,xk);
                int idk = idy + xk;

                int delta_gray = abs(gray_c - *(gray + idk));
                double weight = range_table[delta_gray] * spatial_table[abs(k)];
                sum_div += weight;
                sum += weight * in_[idk];
            }
            out_[idx] = sum / sum_div;
        }
    }
//    cout << "horizontal filtering done.." << endl;

    //vertical filtering
    in_ = temp;
    out_ = out;
    for(int y = 0; y < h; ++y) {
        int idy = y * w;
        for(int x = 0; x < w; ++x) {

            int idx = idy + x;
            unsigned char gray_c = *(gray + idx);

            double sum = 0.0, sum_div = 0.0;
            for(int k = -offset; k <= offset; ++k) {
                int yk = k+y;
                yk = max(0,  yk);
                yk = min(h-1,yk);

                int idk = yk * w + x;
                int delta_gray = abs(gray_c - *(gray + idk));
                double weight = range_table[delta_gray] * spatial_table[abs(k)];
                sum_div += weight;
                sum += weight * in_[idk];
            }
            out_[idx] = sum / sum_div;
        }
    }
//    cout << "vertical filtering done.." << endl;
}

inline void qing_approximated_bilateral_filter(float * out, float * in, unsigned char *  bgr, const int w, const int h, const int wnd, float * range_table, float * spatial_table)
{
   // cout << "qing_approximated_bilateral_filter..." << endl;

    int image_size = h * w;
    int offset = wnd * 0.5;

    float * temp = new float[image_size];
    float * in_ = in;                       /*horizontal filtering*/
    float * out_ = temp;

    for(int y = 0; y < h; ++y) {
        int idy = y * w;
        for(int x = 0; x < w; ++x) {

            int idx = idy + x;
            double sum = 0.0, sum_div = 0.0;

            unsigned char * p_bgr_c = bgr + 3 * idx - 1;
            unsigned char b_c = *(++p_bgr_c);
            unsigned char g_c = *(++p_bgr_c);
            unsigned char r_c = *(++p_bgr_c);

            for(int k = -offset; k <= offset; ++k) {
                int xk = k+x;
                xk = max(0,  xk);
                xk = min(w-1,xk);
                //if(k+x < 0 || k+x >= w) continue;
                //int idk = idx+k;

                int idk = idy + xk;

                unsigned char * p_bgr_k =  bgr + 3 * idk - 1;
                int delta_b = abs(b_c - *(++p_bgr_k));
                int delta_g = abs(g_c - *(++p_bgr_k));
                int delta_r = abs(r_c - *(++p_bgr_k));

                //    double weight = range_table[delta_b] * range_table[delta_g] * range_table[delta_r] * spatial_table[abs(k)];
                int delta_bgr = 0.333333 * (delta_b + delta_g + delta_r);
                double weight = range_table[delta_bgr] * spatial_table[abs(k)];
                sum_div += weight;
                sum += weight * in_[idk];
            }
            out_[idx] = sum / sum_div;
        }
    }
//    cout << "horizontal filtering done.." << endl;

    //vertical filtering
    in_ = temp;
    out_ = out;
    for(int y = 0; y < h; ++y) {
        int idy = y * w;
        for(int x = 0; x < w; ++x) {

            int idx = idy + x;
            double sum = 0.0, sum_div = 0.0;

            unsigned char * p_bgr_c = bgr + 3 * idx - 1;
            unsigned char b_c = *(++p_bgr_c);
            unsigned char g_c = *(++p_bgr_c);
            unsigned char r_c = *(++p_bgr_c);

            for(int k = -offset; k <= offset; ++k) {
                int yk = k+y;
                yk = max(0,  yk);
                yk = min(h-1,yk);
                //if(k+y < 0 || k+y >= h) continue;
                //int idk = idx + k*w;
                int idk = yk * w + x;

                unsigned char * p_bgr_k = bgr + 3 * idk - 1;
                int delta_b = abs(b_c - *(++p_bgr_k));
                int delta_g = abs(g_c - *(++p_bgr_k));
                int delta_r = abs(r_c - *(++p_bgr_k));

                //  double weight = range_table[delta_b] * range_table[delta_g] * range_table[delta_r] * spatial_table[abs(k)] ;

                int delta_bgr = 0.333333 * (delta_b + delta_g + delta_r);
                double weight = range_table[delta_bgr] * spatial_table[abs(k)];
                sum_div += weight;
                sum += weight * in_[idk];
            }
            out_[idx] = sum / sum_div;
        }
    }
//    cout << "vertical filtering done.." << endl;
}


//old: calculate range_table, sigma_table first
inline void qing_approximated_bilateral_filter(float * out, float * in, unsigned char * bgr, const int w, const int h, double sigma_range, double sigma_spatial) {

    cout << "qing_approximated_bilateral_filter..." << endl;

    float * range_table = qing_get_range_weighted_table((int)sigma_range, QING_FILTER_INTENSITY_RANGE);
    float * spatial_table = qing_get_spatial_weighted_table(sigma_spatial, QING_FILTER_SPATIAL_RANGE);
    int wnd = 2*(int)(sigma_spatial+0.5f)+1;
    qing_approximated_bilateral_filter(out, in, bgr, w, h, wnd, range_table, spatial_table);
}

//brute_force
inline void qing_bilateral_filter(float * out, float * in, unsigned char * bgr, const int w, const int h, double sigma_range, double sigma_spatial) {
    //  cout << "qing_bilateral_filter..." << endl;

    int wnd = 2*(int)(sigma_spatial+0.5f)+1;
    int offset = wnd * 0.5;
    float * range_table = qing_get_range_weighted_table(sigma_range, QING_FILTER_INTENSITY_RANGE);
    float * spatial_table = qing_get_spatial_weighted_table(sigma_spatial, 2*(int)(sigma_spatial+0.5f)+1);

    float * in_ = in;
    float * out_ = out;

    for(int y = 0; y < h; ++y) {
        int idy = y * w;
        for(int x = 0; x < w; ++x) {
            int idx = idy + x;

            double sum = 0.0, sum_div = 0.0;

            unsigned char * p_bgr_c = bgr + 3 * idx - 1;
            unsigned char b_c = *(++p_bgr_c);
            unsigned char g_c = *(++p_bgr_c);
            unsigned char r_c = *(++p_bgr_c);

            for(int j = -offset; j <= offset; ++j) {
                if(j+y < 0 || j+y >= h) continue;
                for(int i = -offset; i <= offset; ++i) {
                    if(i+x < 0 || i+x >= w) continue;

                    int idk = idx + j * w + i;
                    unsigned char * p_bgr_k = bgr + 3 * idk - 1;
                    int delta_b = abs(b_c - *(++p_bgr_k));
                    int delta_g = abs(g_c - *(++p_bgr_k));
                    int delta_r = abs(r_c - *(++p_bgr_k));

                    //                    double weight = range_table[delta_b] * range_table[delta_g] * range_table[delta_r] * spatial_table[abs(i)] * spatial_table[abs(j)] ;

                    int delta_bgr = 0.333333 * (delta_b + delta_g + delta_r);
                    double weight = range_table[delta_bgr] * spatial_table[abs(i)] * spatial_table[abs(j)] ;

                    sum_div += weight;
                    sum += weight * in_[idk];
                }
            }
            out_[idx] = sum / sum_div;
        }
    }
}

//brute force of bilateral_filter_one_channel, by vector
inline void qing_bilateral_filter_1ch(const vector<float>& img, const vector<uchar>& msk, const int w, const int h, vector<float>& cost_vol,
                                      const int wnd = 9,  double sigma_spatial = 4.5, double sigma_range = 0.03 ) {
    sigma_spatial = wnd * 0.5;
    int offset = (int)(sigma_range);

    vector<float> dest(w*h, 0.f);

    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            int cen_idx = y * w + x;

            if( 0==msk[cen_idx] ) continue;

            double sum = 0., sum_weight = 0.;
            for(int dy = -offset; dy <= offset; ++dy)
            {
                int cury = (y + dy + h) % h;
                for(int dx = -offset; dx <= offset; ++dx)
                {
                    int curx = (x + dx + w) % w;
                    int cur_idx = cury * w + curx;

                    if( 0==msk[cur_idx] ) continue;

                    double delta_dis_2 = dx * dx + dy * dy;
                    double delta_color_2 = fabs( img[cur_idx] - img[cen_idx] );
                    double weight = exp( -delta_dis_2 / (sigma_spatial * sigma_spatial) - delta_color_2 / (sigma_range * sigma_range) );

                    sum += weight * cost_vol[cur_idx];
                    sum_weight += weight;
                }
            }

            dest[cen_idx] = sum/sum_weight;
        }
    }
    copy(dest.begin(), dest.end(), cost_vol.begin());
}

#endif // QING_BILATERAL_FILTER_H
