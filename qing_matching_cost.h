#ifndef QING_MATCHING_COST_H
#define QING_MATCHING_COST_H

#include "qing_common.h"
#include "qing_string.h"
#include "qing_timer.h"

#define QING_TAD_TRUNCATED      100
#define QING_MAX_MCOST          10000.f
#define QING_MIN_MCOST          -QING_MAX_MCOST

//single pixel matching cost
inline void qing_stereo_flip_cost_vol(vector<vector<vector<float> > >&  hwd_costvol_r, const vector<vector<vector<float> > >&  hwd_costvol_l, int h, int w, int nr_plane) {
    for(int y = 0; y < h; ++y) {
        for(int x = 0; x < w - nr_plane; ++x)
            for(int d = 0; d < nr_plane; ++d) hwd_costvol_r[y][x][d] = hwd_costvol_l[y][x+d][d];

        for(int x = w - nr_plane; x < w; ++x)
            for(int d = 0; d < nr_plane; ++d) {
                if(x+d < w) hwd_costvol_r[y][x][d] = hwd_costvol_l[y][x+d][d];
                else hwd_costvol_r[y][x][d] = hwd_costvol_r[y][x][d-1];
            }
    }
}

inline void qing_wta_disparity(unsigned char * disp, float * cost_vol, int disp_range, int h,  int w, int scale = 1) {

    int image_size = h * w;
    for(int y = 0, idx = 0; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            float min_mcost = QING_MAX_MCOST;
            int min_d = 0;
            for(int d = 1; d < disp_range; ++d) {
                float mcost = *(cost_vol + d * image_size + idx);
                if(mcost < min_mcost) {
                    min_mcost = mcost; min_d = d * scale;
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

//NCC vectors
inline void qing_compute_ncc_vecs(float *** ncc_vecs, const vector<float>& view_sub_mean, const int& h, const int& w, const int& wnd_size) {
    int offset = int(wnd_size * 0.5f);
    int wndsz2 = wnd_size * wnd_size;
    int iy, ix, iidx;
    for(int y = 0; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            memset(ncc_vecs[y][x], 0, sizeof(float)*wndsz2);
            for(int j = -offset; j <= offset; ++j) {
                iy = y + j;   if(iy < 0 || iy > h) continue;
                for(int i = -offset; i <= offset; ++i) {
                    ix = x + i;  if(ix < 0 || ix > w) continue;

                    iidx = (j+offset) * wnd_size + (i+offset);
                    ncc_vecs[y][x][iidx] = view_sub_mean[iy*w+ix];
                }
            }
        }
    }
}

//NCC
inline float qing_ncc_value(const vector<float>& ncc_vec_l, const vector<float>& ncc_vec_r) {
    if(0==ncc_vec_l.size() || 0==ncc_vec_r.size() || ncc_vec_l.size() != ncc_vec_r.size() ) return 0.f;
    double fenzi = 0.0, fenmu1 = 0.0, fenmu2 = 0.0, fenmu = 0.0;

    for(int i = 0; i < ncc_vec_l.size(); ++i) {
        fenzi += ncc_vec_l[i] * ncc_vec_r[i];
        fenmu1 += ncc_vec_l[i] * ncc_vec_l[i];
        fenmu2 += ncc_vec_r[i] * ncc_vec_r[i];
    }

    fenmu = sqrt(fenmu1) * sqrt(fenmu2);
    if(fenmu == 0.0) return 0.1;
    else
        return fenzi/fenmu;
}

//NCC
inline float qing_ncc_value(float * ncc_vec_l, float * ncc_vec_r,  int len) {
    double fenzi = 0.0, fenmu1 = 0.0, fenmu2 = 0.0, fenmu = 0.0;
    for(int i = 0; i < len; ++i) {
        fenzi += ncc_vec_l[i] * ncc_vec_r[i];
        fenmu1 += ncc_vec_l[i] * ncc_vec_l[i];
        fenmu2 += ncc_vec_r[i] * ncc_vec_r[i];
    }

    fenmu = sqrt(fenmu1) * sqrt(fenmu2);
    if(fenmu == 0.0) return 0.0;
    else
        return fenzi/fenmu;
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

inline void qing_save_mcost_vol(const string filename, float * cost_vol, int m_d, int m_h, int m_w) {
    fstream fout(filename.c_str(), ios::out);
    if(fout.is_open() == false) {
        cerr << "failed to open " << filename << endl;
        return ;
    }
    cout << "saving " << filename << endl;
    int m_image_size = m_h * m_w;
    for(int d = 0; d < m_d; ++d) {
        float * mcost = cost_vol + d * m_image_size;
        for(int y = 0, idx = 0; y < m_h; ++y) {
            for(int x = 0; x < m_w; ++x) {
                fout << mcost[idx++] << ' ';
            }
            fout << endl;
        }
        cout << d << ' ';
    }
    cout << endl;    fout.close();
}

#endif // QING_MATCHING_COST_H
