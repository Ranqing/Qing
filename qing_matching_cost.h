#ifndef QING_MATCHING_COST_H
#define QING_MATCHING_COST_H

#include "qing_common.h"
#include "qing_string.h"
#include "qing_timer.h"

#define QING_TAD_TRUNCATED      100
#define QING_MAX_MCOST          10000.f
#define QING_MIN_MCOST          -QING_MAX_MCOST

inline void  qing_wta_disparity(unsigned char * disp, float * cost_vol, int disp_range, int h,  int w, int scale = 1) {

    int image_size = h * w;
    for(int y = 0, idx = 0; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            float min_mcost = QING_MAX_MCOST;
            int min_d = 0;
            for(int d = 1; d < disp_range; ++d) {
                float mcost = *(cost_vol + d * image_size + idx);
                if(mcost < min_mcost) {
                    min_mcost = mcost;
                    min_d = d * scale;
                }
            }
            disp[idx] = min_d;
            idx++;
        }
    }
}

//AD: abs(C_i(x,y) - C_i(x+d, y))
inline unsigned char qing_get_mcost_tad(unsigned char * color_0, unsigned char * color_1, int n = 3) {

    unsigned char delta = abs(*color_0 - *color_1);
    for(int i = 1; i < n; ++i) {
        delta += abs(*(++color_0) - *(++color_1));
    }
    return (unsigned char)min((int)delta, QING_TAD_TRUNCATED);
}

//census from qx
inline unsigned char qx_census_transform_3x3_sub(unsigned char * in, int w) {
    unsigned char census = (*(in-w-1) > *in);
    census+=((*(in-w)>*in)<<1);
    census+=((*(in-w+1)>*in)<<2);
    census+=((*(in-1)>*in)<<3);
    census+=((*(in+1)>*in)<<4);
    census+=((*(in+w-1)>*in)<<5);
    census+=((*(in+w)>*in)<<6);
    census+=((*(in+w+1)>*in)<<7);
    return(census);
}

inline unsigned char qx_hamming_distance(unsigned char x, unsigned char y) {
    unsigned char dist=0, val=x^y;
    while(val){
        dist++;
        val&=val-1;
    }
    return dist;
}
inline void qx_census_transform_3x3(unsigned char * out, unsigned char * in, int h, int w) {
    memset(out, 0, sizeof(unsigned char)*h*w);
    for(int y = 1; y < h-1; ++y) {
        for(int x = 1; x < w-1; ++x) {
            int idx = y*w+x;
            out[idx] = qx_census_transform_3x3_sub(&(in[idx]), w);
        }
    }
}


//matching cost aggregation
//mcost_vol: pixel-level matching cost
//filtered_mcost_vol: filtering matching cost
//len: num of directions
inline void qing_directional_bf_mcost_aggregation(float * filtered_mcost_vol, float * mcost_vol, float * min_mcost_x, unsigned char * gray, int w, int h, int d_range, int wnd, float * range_table, float * spatial_table, float * directions, int len) {

    double * weights = new double[wnd];       //for each direciton, the weights can be pre-computed
    double sum, sum_div, x_dir, y_dir;
    unsigned char gray_c;
    int idy, idx, idk, delta_pq, k_d;
    int image_size = h * w;
    int offset = wnd * 0.5;

    QingTimer timer;

    //x-direction filtering
    cout << "horizontal filtering start.." << endl;

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
    cout << "vertical filtering start..." << endl;
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
                    delta_pq = abs(gray_c - *(gray+ idk));
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


//debug
inline void qing_save_mcost_txt(const string filename, float * mcost, int total_size, int step) {
    fstream fout(filename, ios::out);
    if(fout.is_open() == false) {
        cerr << "failed to open " << filename << endl;
        return ;
    }
    for(int i = 0, step_count = 0; i < total_size; ++i) {
        fout << mcost[i] << ' ';
        step_count ++;
        if(step_count == step) {
            step_count = 0;
            fout << endl;
        }
    }
    cout << "saving " << filename << endl;
}

inline Mat qing_save_mcost_jpg(const string filename, float * mcost, int w, int h) {
    int image_size = w * h;

    Mat mcost_mat(h, w, CV_32FC1);
    memcpy(mcost_mat.ptr<float>(0), mcost, sizeof(float)*image_size);

    double min_val, max_val;
    cv::minMaxIdx(mcost_mat, &min_val, &max_val);
    double scale = min(1/min_val, 255/max_val);
    Mat vis_mcost_mat(h, w, CV_8UC1);
    mcost_mat.convertTo(vis_mcost_mat, CV_8UC1, scale);
    imwrite(filename, vis_mcost_mat);
    cout << "saving " << filename << endl;
    return vis_mcost_mat;
}

inline Mat qing_save_mcost_jpg_inf(const string filename, float * mcost, int w, int h) {
    int image_size = w * h;

    Mat mcost_mat = Mat::zeros(h, w, CV_32FC1);
    float * ptr_mcost_mat = mcost_mat.ptr<float>(0);
    memcpy(ptr_mcost_mat, mcost, sizeof(float)*image_size);

    for(int i = 0; i < image_size; ++i) {
        if(QING_MAX_MCOST <= ptr_mcost_mat[i])    ptr_mcost_mat[i] = 0.f;
    }

    double min_val, max_val;
    cv::minMaxIdx(mcost_mat, &min_val, &max_val);

    double scale = min(1/min_val, 255/max_val);
# if 1
    cout << "maxval = " << max_val << "\t scale  = " << scale << endl;
# endif

    Mat vis_mcost_mat(h, w, CV_8UC1);
    mcost_mat.convertTo(vis_mcost_mat, CV_8UC1, scale);
    imwrite(filename, vis_mcost_mat);
    cout << "saving " << filename << endl;
    return vis_mcost_mat;
}

#endif // QING_MATCHING_COST_H