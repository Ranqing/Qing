#ifndef QING_MATCHING_COST_H
#define QING_MATCHING_COST_H

#include "qing_common.h"
#include "qing_string.h"

#define QING_TAD_TRUNCATED 40

inline unsigned char qing_get_mcost_tad(unsigned char * color_0, unsigned char * color_1, int n = 3) {

    unsigned char delta = abs(*color_0 - *color_1);
    for(int i = 1; i < n; ++i) {
        delta += abs(*(++color_0) - *(++color_1));
    }
    return (unsigned char)min((int)delta, QING_TAD_TRUNCATED);
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

inline void qing_save_mcost_jpg(const string filename, float * mcost, int w, int h) {
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
}

#endif // QING_MATCHING_COST_H
