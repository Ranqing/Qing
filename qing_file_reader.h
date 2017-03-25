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

inline void qing_read_disp_txt(const string disp_file, const int h, const int w, vector<float>& disp) {
    int total_size = h*w;
    disp.resize(total_size);
    fstream fin(disp_file.c_str(), ios::in);
    if(fin.is_open() == false) {
        cerr << "failed to open " << disp_file << endl;
        exit(1);
    }
    for(int idx = 0; idx < total_size; ++idx ) {
        fin >> disp[idx];
    }
}

inline void qing_read_txt(const string& filename, vector<string>& contents)
{
    fstream fin(filename.c_str(), ios::in);
    if(fin.is_open() == false)
    {
        cerr << "failed to open " << filename << endl;
        return ;
    }
    string s;
    while(fin >> s)
    {
        contents.push_back(s);
    }
#if 1
    cout << "end of reading " << filename << ", " << contents.size() << " lines." << endl;
#endif
}

//read in strings in format xml
inline void qing_read_strings_xml(const string& filename, vector<string>& contents)
{
    contents.resize(0);
    FileStorage fs(filename, FileStorage::READ);
    if (!fs.isOpened())
        return ;

    FileNode n = fs.getFirstTopLevelNode();
    FileNodeIterator it = n.begin(), it_end = n.end();
    for (; it != it_end; it++)
        contents.push_back((string)* it);
    cout << "end of reading " << filename << ", " << contents.size() << " lines." << endl;
}

inline void qing_read_intrinsic_yml(const string& filename, Mat& camera_matrix, Mat& dist_coeffs)
{
    FileStorage fs(filename, FileStorage::READ);

    fs["Camera_Matrix"] >> camera_matrix;
    fs["Distortion_Coefficients"] >> dist_coeffs;

    fs.release();
}

//stereoFile, R0, R1, P0, P1, Q, imageSize
inline void qing_read_stereo_yml(const string& filename, Mat& R1, Mat& R2, Mat& P1, Mat& P2, Mat& Q, Size& imageSize )
{
    FileStorage fs(filename, FileStorage::READ);
    if(false == fs.isOpened())
    {
        cerr << "failed to open " << filename << endl;
        return ;
    }

    fs["Q"] >> Q;
    fs["R1"] >> R1 ;
    fs["R2"] >> R2;
    fs["P1"] >> P1;
    fs["P2"] >> P2;
    fs["Image_Width"]  >> imageSize.width;
    fs["Image_Height"] >> imageSize.height;

    fs.release();
}

//stereoFile, Q
inline void qing_read_stereo_yml_qmatrix(const string& filename, Mat& Q)
{
    FileStorage fs(filename, FileStorage::READ);
    if(false == fs.isOpened())
    {
        cerr << "failed to open " << filename << endl;
        return ;
    }

    fs["Q"] >> Q;
    fs.release();
}

//stereoFile, R, t
inline void qing_read_stereo_yml_rt(const string& filename, Mat& R, Mat& t)
{
    FileStorage fs(filename, FileStorage::READ);
    if(false == fs.isOpened())
    {
        cerr << "failed to open " << filename << endl;
        return ;
    }

    fs["Rotation_Matrix"] >> R;
    fs["Translation_Vector"] >> t;

    fs.release();
}


#endif // QING_FILE_READER

