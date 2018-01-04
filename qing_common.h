#ifndef QING_COMMON_H
#define QING_COMMON_H

//unix related
//#ifdef linux
//#include <unistd.h>
//#include <dirent.h>
//#include <fcntl.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/mman.h>
//#endif
//#ifdef _WIN32
//#include <direct.h>
//#include <io.h>
//#endif

//win32/win64 related
#ifdef _WIN32
#include <direct.h>
#include <io.h>
//unix/osx related
#else
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#endif

//c header
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

//C++ header
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>
#include <limits>
#include <bitset>
using namespace std;

//PCL header
//#include <pcl/io/pcd_io.h>
//#include <pcl/io/ply_io.h>
//#include <pcl/io/vtk_io.h>
//#include <pcl/point_types.h>
//#include <pcl/filters/voxel_grid.h>
//#include <pcl/point_types.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl/kdtree/kdtree_flann.h>
//#include <pcl/features/normal_3d.h>
//#include <pcl/surface/gp3.h>

//#include <pcl/common/transforms.h>
//#include <pcl/common/pca.h>
//#include <pcl/filters/crop_box.h>
//#include <pcl/visualization/point_cloud_color_handlers.h>
//#include <pcl/visualization/pcl_visualizer.h>
//#include <pcl/surface/convex_hull.h>
//#include <pcl/sample_consensus/method_types.h>
//#include <pcl/sample_consensus/model_types.h>
//#include <pcl/segmentation/sac_segmentation.h>
//#include <pcl/segmentation/extract_clusters.h>
//#include <pcl/visualization/image_viewer.h>
//#include <pcl/features/moment_of_inertia_estimation.h>
//#include <pcl/features/normal_3d.h>
//#include <pcl/kdtree/kdtree.h>
//#include <pcl/filters/extract_indices.h>
//#include <pcl/filters/voxel_grid.h>

//OpenCV header
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/imgcodecs/imgcodecs.hpp>
//#include <opencv2/videoio/videoio.hpp>
//#include<opencv2/nonfree/nonfree.hpp>
//#include<opencv2/legacy/legacy.hpp>
using namespace cv;

//Eigen
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigen>
#define DOUBLE_MAX 1e10
#define QING_PI 3.14159265358979323846
#endif
