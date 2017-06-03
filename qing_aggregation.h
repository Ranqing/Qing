#ifndef QING_AGGREGATION_H
#define QING_AGGREGATION_H

#include "qing_common.h"
#include "qing_bilateral_filter.h"

//aggregate mcost volume with approximate bilateral filter guided by gray image
inline void qing_bf_mcost_aggregation(float * filtered_mcost_vol, float * mcost_vol, unsigned char *gray, int w, int h, int d_range, int wnd, float * range_table, float *spatial_table) {

    int image_size = h * w;
    int total_size = d_range * image_size;
    memcpy(filtered_mcost_vol, mcost_vol, sizeof(float)*total_size);

    QingTimer timer;
    for(int d = 0; d < d_range; ++d) {
        float * out = filtered_mcost_vol + d * image_size; timer.restart();
        //qing_approximated_gray_bilateral_filter(out, out, gray, w, h, wnd, range_table, spatial_table);
        cout << "d = " << d << "\t duration = " << timer.duration()*1000 << " ms." << endl;
# if 0
        string out_file = "directional_matching_cost/bf_filtered_mcost_" + qing_int_2_string(d) + ".jpg";
        qing_save_mcost_jpg(out_file, out, m_w, m_h);
# endif
    }
}

//mcost_vol: raw matching cost
//filtered_mcost_vol: filtering matching cost
//len: num of directions
//directional aggregate mcost volume with approximated bilateral filter guided by gray image
//bf: bilateral filter
inline void qing_directional_bf_mcost_aggregation(float * filtered_mcost_vol, float * mcost_vol, float * min_mcost_x, unsigned char * gray,
                                                  int w, int h, int d_range, int wnd, float * range_table, float * spatial_table, float * directions, int len) {

    double * weights = new double[wnd];       //for each direciton, the weights can be pre-computed
    double sum, sum_div, x_dir, y_dir;
    unsigned char gray_c;
    int idy, idx, idk, delta_pq, k_d;
    int image_size = h * w;
    int offset = wnd * 0.5;

    QingTimer timer;

    //x-direction filtering
    cout << "horizontal filtering start.." ;

    //different window size
    wnd = 21;
    offset = wnd * 0.5;
    cout << "\twnd = " << wnd << endl;

    for(int d = 0; d < d_range; ++d) {
        float * out = min_mcost_x + d * image_size;
        cout << "d = " << d ; timer.restart();

        for(int y = 0; y < h; ++y) {
            idy = y * w;
            for(int x = 0; x < w; ++x) {
                idx = idy + x;
                gray_c = *(gray + idx);

                std::fill(weights, weights + wnd, 0.0);
                for(int k = -offset, tk = 0; k <= offset; ++tk, ++k) {
                    if(x+k < 0 || x+k >= w) continue;
                    idk = idx + k;
                    delta_pq = abs(gray_c - *(gray + idk));
                    weights[tk] = range_table[delta_pq] * spatial_table[abs(k)];
                }

                //compute aggregate matching cost in this horizontal support window of each directions
                //select the minimum , stored in out[idx], i.e, min_mcost_x[d*m_image_size+idx]
                for(int wx = 0; wx < len; ++wx) {
                    x_dir = directions[wx];
                    sum = 0.0; sum_div = 0.0;

                    for(int k = -offset, tk = 0; k<=offset; ++tk,++k) {
                        if(x+k < 0 || x+k >= w) continue;
                        idk = idx + k;

                        k_d = d + x_dir * k;
                        k_d = max(0,k_d);
                        k_d = min(k_d, d_range-1);

                        sum += weights[tk] * (*(mcost_vol + k_d * image_size + idk));
                        sum_div += weights[tk];
                    }
                    if(sum_div >= 0.000001) sum/=sum_div;
                    if(sum < out[idx]) { out[idx] = sum; }
                }
            }
        }
        cout << "\t duration = " << timer.duration() * 1000 << " ms." << endl;
# if 0
        qing_create_dir("bf_aggr_matching_cost");
        string out_file = "bf_aggr_matching_cost/x_mcost_" + qing_int_2_string(d) + ".jpg";
        qing_save_mcost_jpg(out_file, out, m_w, m_h);
        string out_txt_file = "bf_aggr_matching_cost/x_mcost_" + qing_int_2_string(d) + ".txt";
        qing_save_mcost_txt(out_txt_file, out, m_image_size, m_w);
# endif
    }
    cout << "horizontal filtering end..." << endl;
# if 1
    Mat x_disp_mat = Mat::zeros(h, w, CV_8UC1);
    unsigned char * ptr = x_disp_mat.ptr<unsigned char>(0);
    qing_wta_disparity(ptr, min_mcost_x, d_range, h, w, 255/d_range);
    string file = "../disp_in_x.png";
    imwrite(file, x_disp_mat); cout << "saving " << file << endl;
# endif


    //y-direction filtering
    cout << "vertical filtering start...\t";
    wnd = 11;
    offset = wnd * 0.5;
    cout << "wnd = " << wnd << endl;
    for(int d = 0; d < d_range; ++d) {
        float * out = filtered_mcost_vol + d * image_size;
        cout << "d = " << d ; timer.restart();

        for(int y = 0; y < h; ++y) {
            idy = y * w;
            for(int x = 0; x < w; ++x) {
                idx = idy + x;
                gray_c = *(gray + idx);

                std::fill(weights, weights+wnd, 0.0);
                for(int k = -offset, tk = 0; k <= offset; ++tk, ++k) {
                    if(y+k < 0 || y+k >= h) continue;

                    idk = idx + k * w;
                    delta_pq = abs(gray_c - *(gray + idk));
                    weights[tk] = range_table[delta_pq] * spatial_table[abs(k)];
                }

                //compute aggregate matching cost in this vertical support window of each directions
                //select the minumum, stored in out[idx], i.e, m_filtered_mcost_l[d*m_image_size+idx]

                for(int wy = 0; wy < len; ++wy) {
                    y_dir = directions[wy];
                    sum = 0.0; sum_div = 0.0;

                    for(int k = -offset, tk = 0; k<=offset; ++tk, ++k) {
                        if(y+k < 0 || y+k >= w) continue;
                        idk = idx + k * w;
                        k_d = d + y_dir * k;
                        k_d = max(0,k_d);
                        k_d = min(k_d, d_range-1);

                        sum += weights[tk] * (*(min_mcost_x + k_d * image_size + idk));
                        sum_div += weights[tk];
                    }

                    if(sum_div >= 0.0000001) sum/=sum_div;
                    if(sum < out[idx]) {out[idx] = sum; }
                }
            }
        }
        cout << "\t duration = " << timer.duration() * 1000 << " ms." << endl;
# if 0
        qing_create_dir("bf_aggr_matching_cost");
        string out_file = "bf_aggr_matching_cost/y_mcost_" + qing_int_2_string(d) + ".jpg";
        qing_save_mcost_jpg(out_file, out, m_w, m_h);
        string out_txt_file = "bf_aggr_matching_cost/y_mcost_" + qing_int_2_string(d) + ".txt";
        qing_save_mcost_txt(out_txt_file, out, m_image_size, m_w);
# endif
    }
    cout << "vertial filtering end...." << endl;
# if 1
    Mat y_disp_mat = Mat::zeros(h, w, CV_8UC1);
    ptr = y_disp_mat.ptr<unsigned char>(0);
    qing_wta_disparity(ptr, filtered_mcost_vol, d_range, h, w, 255/d_range);
    file = "../disp_in_y.png";
    imwrite(file, y_disp_mat); cout << "saving " << file << endl;
# endif
}

//aw: adaptive weight
inline void qing_directional_aw_mcost_aggregation_l(float * filtered_mcost_vol, float * mcost_vol, float * min_mcost_x, unsigned char * gray_l, unsigned char * gray_r,
                                                    int w, int h, int d_range, int wnd, float * range_table, float * spatial_table, float * directions, int len) {

    double * weights = new double[wnd];       //for each direciton, the weights can be pre-computed
    double sum, sum_div, x_dir, y_dir;
    unsigned char gray_c;
    int idy, idx, idk, delta_pq, delta_pq_d, k_d;
    int image_size = h * w;
    int offset = wnd * 0.5;

    QingTimer timer;

    //x-direction filtering
    cout << "horizontal filtering start.." << endl;

    //different window size
    wnd = 21;
    offset = wnd * 0.5;
    cout << "wnd size = " << wnd << endl;

    for(int d = 0; d < d_range; ++d) {
        float * out = min_mcost_x + d * image_size;
        cout << "d = " << d ; timer.restart();

        for(int y = 0; y < h; ++y) {
            idy = y * w;
            for(int x = 0; x < w; ++x) {
                if(x-d < 0) continue;

                idx = idy + x;
                gray_c = *(gray_l + idx);

                std::fill(weights, weights + wnd, 0.0);
                for(int k = -offset, tk = 0; k <= offset; ++tk, ++k) {
                    if(x+k < 0 || x+k >= w) continue;
                    idk = idx + k;
                    delta_pq = abs(gray_c - *(gray_l + idk));
                    weights[tk] = range_table[delta_pq] * spatial_table[abs(k)] * spatial_table[abs(k)];
                }

                //compute aggregate matching cost in this horizontal support window of each directions
                //select the minimum , stored in out[idx], i.e, min_mcost_x[d*m_image_size+idx]
                for(int wx = 0; wx < len; ++wx) {
                    x_dir = directions[wx];
                    sum = 0.0; sum_div = 0.0;

                    for(int k = -offset, tk = 0; k<=offset; ++tk,++k) {
                        if(x+k < 0 || x+k >= w) continue;
                        idk = idx + k;

                        k_d = d + x_dir * k;
                        k_d = max(0,k_d);
                        k_d = min(k_d, d_range-1);

                        if((x+k)-k_d < 0) continue;

                        delta_pq_d = abs( *(gray_r+idx-d) - *(gray_r+idk-k_d) );

                        weights[tk] = weights[tk] * range_table[delta_pq_d];
                        sum += weights[tk] * (*(mcost_vol + k_d * image_size + idk));
                        sum_div += weights[tk];
                    }
                    if(sum_div >= 0.000001) sum/=sum_div;
                    if(sum < out[idx]) { out[idx] = sum; }
                }
            }
        }
        cout << "\t duration = " << timer.duration() * 1000 << " ms." << endl;
# if 0
        qing_create_dir("bf_aggr_matching_cost");
        string out_file = "bf_aggr_matching_cost/x_mcost_" + qing_int_2_string(d) + ".jpg";
        qing_save_mcost_jpg(out_file, out, m_w, m_h);
        string out_txt_file = "bf_aggr_matching_cost/x_mcost_" + qing_int_2_string(d) + ".txt";
        qing_save_mcost_txt(out_txt_file, out, m_image_size, m_w);
# endif
    }
    cout << "horizontal filtering end..." << endl;
# if 1
    Mat x_disp_mat = Mat::zeros(h, w, CV_8UC1);
    unsigned char * ptr = x_disp_mat.ptr<unsigned char>(0);
    qing_wta_disparity(ptr, min_mcost_x, d_range, h, w, 255/d_range);
    string file = "../disp_in_x.png";
    imwrite(file, x_disp_mat); cout << "saving " << file << endl;
# endif

    //y-direction filtering
    cout << "vertical filtering start..." << endl;
    wnd = 11;
    offset = wnd * 0.5;
    cout << "wnd size = " << wnd << endl;
    for(int d = 0; d < d_range; ++d) {
        float * out = filtered_mcost_vol + d * image_size;
        cout << "d = " << d ; timer.restart();

        for(int y = 0; y < h; ++y) {
            idy = y * w;
            for(int x = 0; x < w; ++x) {
                idx = idy + x;      if(x-d<0) continue;
                gray_c = *(gray_l + idx);

                std::fill(weights, weights+wnd, 0.0);
                for(int k = -offset, tk = 0; k <= offset; ++tk, ++k) {
                    if(y+k < 0 || y+k >= h) continue;

                    idk = idx + k * w;
                    delta_pq = abs(gray_c - *(gray_l + idk));
                    weights[tk] = range_table[delta_pq] * spatial_table[abs(k)] * spatial_table[abs(k)];
                }

                //compute aggregate matching cost in this vertical support window of each directions
                //select the minumum, stored in out[idx], i.e, m_filtered_mcost_l[d*m_image_size+idx]

                for(int wy = 0; wy < len; ++wy) {
                    y_dir = directions[wy];
                    sum = 0.0; sum_div = 0.0;

                    for(int k = -offset, tk = 0; k<=offset; ++tk, ++k) {
                        if(y+k < 0 || y+k >= w) continue;
                        idk = idx + k * w;
                        k_d = d + y_dir * k;
                        k_d = max(0,k_d);
                        k_d = min(k_d, d_range-1);

                        if(x-k_d<0)continue;

                        delta_pq_d = abs(*(gray_r+idx-d) - *(gray_r+idk-k_d));

                        weights[tk] *= range_table[delta_pq_d];
                        sum += weights[tk] * (*(min_mcost_x + k_d * image_size + idk));
                        sum_div += weights[tk];
                    }

                    if(sum_div >= 0.0000001) sum/=sum_div;
                    if(sum < out[idx]) {out[idx] = sum; }
                }
            }
        }
        cout << "\t duration = " << timer.duration() * 1000 << " ms." << endl;
# if 0
        qing_create_dir("bf_aggr_matching_cost");
        string out_file = "bf_aggr_matching_cost/y_mcost_" + qing_int_2_string(d) + ".jpg";
        qing_save_mcost_jpg(out_file, out, m_w, m_h);
        string out_txt_file = "bf_aggr_matching_cost/y_mcost_" + qing_int_2_string(d) + ".txt";
        qing_save_mcost_txt(out_txt_file, out, m_image_size, m_w);
# endif
    }
    cout << "vertial filtering end...." << endl;
# if 1
    Mat y_disp_mat = Mat::zeros(h, w, CV_8UC1);
    ptr = y_disp_mat.ptr<unsigned char>(0);
    qing_wta_disparity(ptr, filtered_mcost_vol, d_range, h, w, 255/d_range);
    file = "../disp_in_y.png";
    imwrite(file, y_disp_mat); cout << "saving " << file << endl;
# endif
}

//using two images as guidance
//not finished yet
inline void qing_directional_bf_mcost_aggregation(float *filtered_mcost_vol, float * mcost_vol, float *min_mcost_x, unsigned char *gray_l, unsigned char * gray_r, int w, int h, int d_range, int wnd, float *range_table, float *spatial_table, float *directions, int len) {

    double * weights = new double[wnd];       //for each direciton, the weights can be pre-computed
    double sum, sum_div, x_dir, y_dir;
    unsigned char gray_c;
    int idy, idx, idk, delta_pq, k_d;
    int image_size = h * w;
    int offset = wnd * 0.5;
}

#endif // QING_AGGREGATION_H
