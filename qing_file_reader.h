#ifndef QING_FILE_READER
#define QING_FILE_READER

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

//data's memory should be alloc first
inline void qing_read_bin_c(const string filename, float *& data, int total_size) {
    FILE * fin_in = fopen(filename.c_str(),  "rb");
    if(0 == fin_in) {
        cerr << "failed to open " << filename << endl;
        return ;
    }

    float buf;
    int cnt = 0;
    while(0 != (fread(&buf, sizeof(float), 1, fin_in))) {
        data[cnt++] = buf;
        if(cnt >= total_size) break;
   }
   fclose(fin_in);
}

inline void qing_read_bin(const string filename, float *& data, int total_size) {
    ifstream fin(filename.c_str(), ios::in|ios::binary);
    if(false == fin.is_open()) {
        cerr << "error to open " << filename << endl;
        return ;
    }

    fin.read((char *)(data), sizeof(float)*total_size);

# if 0
    //read in data one-by-one
    for(int i = 0; i < total_size; ++i) {
        fin.read((char *)(data + i), sizeof(float));
    }
# endif
}

inline void qing_read_txt(const string filename, float *& data, int total_size) {
    fstream fin(filename.c_str(), ios::in);
    if(false == fin.is_open()) {
        cerr << "error to open "  << filename << endl;
        return ;
    }

    for(int i = 0; i < total_size; ++i) {
        fin >> data[i];
    }
    fin.close();
    cout << "reading " << filename << " done." << endl;
}

inline Mat qing_read_disp_txt(const string disp_file, Size disp_size) {
    Mat disp_mat = Mat::zeros(disp_size, CV_32FC1);
    int disp_h = disp_size.width;
    int disp_w = disp_size.height;
    fstream fin(disp_file.c_str(), ios::in);
    if(fin.is_open() == false) {
        cerr << "failed to open " << disp_file << endl;
        exit(1);
    }

    float * ptr = disp_mat.ptr<float>(0);
    for(int y = 0, idx = 0; y < disp_h; ++y) {
        for(int x = 0; x < disp_w; ++x) {
            fin >> ptr[idx++];
        }
    }
    cout << "finish reading " << disp_file << endl;
    return disp_mat;
}


#endif // QING_FILE_READER

