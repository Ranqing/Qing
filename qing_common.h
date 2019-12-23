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

using std::string;
using std::vector;

//Eigen
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigen>

#define DOUBLE_MAX 1e10
#define QING_PI 3.14159265358979323846
#endif
