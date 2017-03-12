#ifndef QING_FILE_WRITER_H
#define QING_FILE_WRITER_H

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

//total_size: length of data
//step_size:  length of output '\n'
inline void qing_write_txt(const string filename, const float* data, const int total_size, const int step_size = 0) {
    ofstream fout(filename, ios_base::out) ;
    if(false == fout.is_open()) {
        cerr << "failed to open " << filename << endl;
        return ;
    }

    int n_steps = 0;
    for(int i = 0, counter = 0; i < total_size; ++i) {
        fout << data[i] << ' ';
        counter ++;
        if(counter == step_size) {
            fout << endl;
            counter = 0;
            n_steps ++;
        }
    }
    fout << endl;
    fout.close();
    cout << "writing " << filename << endl;
}

inline void qing_write_bin_c(const string filename, float *& data, int total_size) {
    FILE * p_file = fopen (filename.c_str(), "wb");
    if(0==p_file) {
        cerr << "failed to open " << filename << endl;
    }
    fwrite(data, sizeof(float), total_size, p_file);
    fclose (p_file);
    cout << "writing " << filename << " done." << endl;
}

inline void qing_write_bin(const string filename, float *& data, int total_size) {
    fstream fout(filename.c_str(), ios::out|ios::binary);
    if(false == fout.is_open()) {
        cerr << "error to open " << filename << endl;
        return ;
    }
    fout.write((char *)data, sizeof(float)*total_size);
    fout.close();
    cout << "writing " << filename << " done." << endl;
}


#endif // QING_FILE_WRITER_H
