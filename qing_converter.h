#ifndef QING_CONVERTER_H
#define QING_CONVERTER_H

#include "qing_common.h"

//inline void qing_point2f_to_mat(const vector<Point2f>& points, Mat& matrix)
//{
//    int width = matrix.size().width;
//    int height = matrix.size().height;
//    int ch = matrix.channels();

//    double *  ptr = matrix.ptr<double>(0);
//    for(int i = 0; i < height; ++i)
//    {
//        for(int j = 0; j < width; ++j)
//        {
//            int index = i * width + j;
//            ptr[ index * ch ] = points[index].x;
//            ptr[ index * ch + 1] = points[index].y;
//        }
//    }

//#if 0
//    cout << "check " << endl;
//    for(int i = 0; i < points.size(); ++i)
//    {
//        cout << points[i] << endl;
//    }
//    cout << endl;
//    cout << matrix << endl;
//#endif
//}

//inline void qing_mat_to_point3f(const Mat& matrix, vector<Point3f>& points)
//{
//    int width = matrix.size().width;
//    int height = matrix.size().height;
//    int ch = matrix.channels();

//    double *  ptr = (double *)matrix.ptr<double>(0);
//    for(int i = 0; i < height; ++i)
//    {
//        for(int j = 0; j < width; ++j)
//        {
//            int index = i * width + j;
//            double x = ptr[index * ch + 0];
//            double y = ptr[index * ch + 1];
//            double z = ptr[index * ch + 2];
//            double d = ptr[index * ch + 3];
//            points[index] = Point3f ( x/d, y/d, z/d );
//        }
//    }

//#if 1
//    cout << "check " << endl;
//    for(int i = 0; i < points.size(); ++i)
//    {
//        cout << points[i] << endl;
//    }
//    cout << "end of check " << endl;
//#endif
//}

#endif // QING_CONVERTER_H
