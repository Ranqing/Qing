#ifndef QING_POINTCLOUD_H
#define QING_POINTCLOUD_H

#include "qing_common.h"

typedef pcl::PointXYZ QingPointXYZ;
typedef pcl::PointCloud<pcl::PointXYZ> QingPointcloudXYZ;
typedef pcl::PointCloud<pcl::PointXYZ>::Ptr QingPointcloudXYZPtr;

typedef pcl::PointXYZRGBNormal QingPointXYZRgbN;
typedef pcl::PointCloud<pcl::PointXYZRGBNormal> QingPointcloudXYZRgbN;
typedef pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr QingPointcloudXYZRgbNPtr;

inline void qing_read_pointxyz_ply(const string filename, QingPointcloudXYZPtr cloud)
{
    if(filename.find(".ply") == -1)
    {
        cerr << "failed to open " << filename << endl;
        return ;
    }

    // Fill in the cloud data
    pcl::PLYReader reader;
    reader.read (filename, *cloud); // Remember to download the file first!
}

//inline void qing_read_pointxyz_xyz(const string filename, QingPointcloudXYZPtr cloud)
//{
//    if(filename.substr(".xyz") == -1)
//    {
//        cerr << "failed to open " << filename << endl;
//        return ;
//    }



//}

#endif // QING_POINTCLOUD_H
