#ifndef QING_FILE_WRITER_H
#define QING_FILE_WRITER_H

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

//total_size: length of data
//step_size:  length of output '\n'
inline void qing_write_txt (const string filename, const float *data, const int total_size, const int step_size = 0) {
    ofstream fout(filename.c_str(), ios_base::out);
    if (false == fout.is_open()) {
        cerr << "failed to open " << filename << endl;
        return;
    }

    int n_steps = 0;
    for (int i = 0, counter = 0; i < total_size; ++i) {
        fout << data[i] << ' ';
        counter++;
        if (counter == step_size) {
            fout << endl;
            counter = 0;
            n_steps++;
        }
    }
    fout << endl;
    fout.close();
    cout << "writing " << filename << endl;
}

inline void qing_write_bin_c (const string filename, float *&data, int total_size) {
    FILE *p_file = fopen(filename.c_str(), "wb");
    if (0 == p_file) {
        cerr << "failed to open " << filename << endl;
    }
    fwrite(data, sizeof(float), total_size, p_file);
    fclose(p_file);
    cout << "writing " << filename << " done." << endl;
}

inline void qing_write_bin (const string filename, float *&data, int total_size) {
    fstream fout(filename.c_str(), ios::out | ios::binary);
    if (false == fout.is_open()) {
        cerr << "error to open " << filename << endl;
        return;
    }
    fout.write((char *) data, sizeof(float) * total_size);
    fout.close();
    cout << "writing " << filename << " done." << endl;
}

//(savefn, i, cam0, cam1, imageSize, Point2f(0.f,0.f), Point2f(0.f, 0.f), 240.0f, 0.f, Q);
//stereoname
//cam0
//cam1
//img0
//img1
//cropxy0
//cropxy1
//cropsz
//MAXDISP
//MINDISP
//Qmatrix
//filename, camName0, camName1, imgName0, imgName1, cxy[idx0], cxy[idx1], csz[idx0], MAX_DISP, MIN_DISP, Qmatrix
inline void qing_write_stereo_info (const string &filename, const int stereoidx,
                                    const string &cam0, const string &cam1,
                                    const string &frameName,
                                    const string &img0, const string &img1,
                                    const string &msk0, const string &msk1,
                                    const Point2f pt0, const Point2f pt1,
                                    const Size imageSize,
                                    const float maxDisp, const float minDisp, const Mat &Qmatrix) {
    fstream fout(filename.c_str(), ios::out);
    if (fout.is_open() == false) {
        cerr << "failed to open " << filename << endl;
        return;
    }

# if 0  // old
    fout << cam0 + cam1 << endl;
    fout << i << endl;
    fout << cam0 << endl;
    fout << cam1 << endl;
    fout << pt0.x << " " << pt0.y << endl;   // left start point
    fout << pt1.x << " " << pt1.y << endl;   // right start point
    fout << imageSize.width << " " << imageSize.height << endl;  //image size
    fout << maxDisp << endl;    //maxdisp
    fout << minDisp << endl;    //mindisp

    double * ptr = (double *)Qmatrix.ptr(0);
    for(int r = 0; r < 4; r++)
    {
        for(int c = 0; c < 4; c++ )
        {
            //fout 格式化输出
            fout << setprecision(16) << ptr[r*4+c] << ' ';
        }
        fout << endl;
    }
    fout.close();
#endif

#if 1
    fout << cam0 + cam1 << endl;
    fout << stereoidx << endl;
    fout << cam0 << endl;
    fout << cam1 << endl;
    fout << frameName << endl;
    fout << img0 << endl;
    fout << img1 << endl;
    fout << msk0 << endl;
    fout << msk1 << endl;
    fout << pt0.x << " " << pt0.y << endl;   // left start point
    fout << pt1.x << " " << pt1.y << endl;   // right start point
    fout << imageSize.width << " " << imageSize.height << endl;  //image size
    fout << maxDisp << endl;    //maxdisp
    fout << minDisp << endl;    //mindisp

    double *ptr = (double *) Qmatrix.ptr(0);
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            //fout 格式化输出
            fout << setprecision(16) << ptr[r * 4 + c] << ' ';
        }
        fout << endl;
    }
    fout.close();
#endif
}

inline void qing_write_p_matrices (const string filename, const Mat &P1, const Mat &P2) {
    fstream fout(filename.c_str(), ios::out);
    if (false == fout.is_open()) {
        cerr << "failed to open " << filename << endl;
        return;
    }
    double *ptr_1 = (double *) P1.ptr<double>(0);
    double *ptr_2 = (double *) P2.ptr<double>(0);
    fout << "P1:\t";
    for (int i = 0; i < 12; ++i) {
        fout << setprecision(10) << ptr_1[i] << '\t';
    }
    fout << endl;
    fout << "P2:\t";
    for (int i = 0; i < 12; ++i) {
        fout << setprecision(10) << ptr_2[i] << '\t';
    }
    fout << endl;
    return;
}


#endif // QING_FILE_WRITER_H
