#ifndef QING_IO_H
#define QING_IO_H

#include "qing_common.h"

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

inline void qing_read_xml(const string& filename, vector<string>& contents)
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

    fs["Q"] >> Q;
    fs["R1"] >> R1 ;
    fs["R2"] >> R2;
    fs["P1"] >> P1;
    fs["P2"] >> P2;
    fs["Image_Width"]  >> imageSize.width;
    fs["Image_Height"] >> imageSize.height;

    fs.release();
}

//stereoFile, R0, R1, P0, P1, Q, imageSize
inline void qing_read_stereo_yml_qmatrix(const string& filename, Mat& Q)
{
    FileStorage fs(filename, FileStorage::READ);

    fs["Q"] >> Q;
    fs.release();
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
inline void qing_write_stereo_info(const string& filename, const int stereoidx,
                                   const string& cam0, const string& cam1,
                                   const string& frameName,
                                   const string& img0, const string& img1,
                                   const string& msk0, const string& msk1,
                                   const Point2f pt0, const Point2f pt1,
                                   const Size imageSize,
                                   const float maxDisp, const float minDisp, const Mat& Qmatrix )
{
    fstream fout(filename.c_str(), ios::out);
    if(fout.is_open() == false)
    {
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
    fout << stereoidx   << endl;
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
}



#endif // QING_IO_H
