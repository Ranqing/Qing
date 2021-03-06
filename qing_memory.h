#ifndef QING_MEMORY_H
#define QING_MEMORY_H

#include "qx_memory.h"

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

template <typename T>
T *  qing_new_1d_array(T * ptr, int size) {  ptr = new T[size];  return ptr; }

template <typename T>
T * qing_clear_1d_array(T * ptr) {
    if(NULL != ptr) { delete[] ptr; ptr = NULL; }
    return ptr;
}

//be a 2d-array class
template <typename T>
T ** qing_new_2d_array(T ** ptr, int size1, int size2) {
    ptr = new T*[size1];
    for(int i = 0; i < size1; ++i)  ptr[i] = new T[size2];
    return ptr;
}

template <typename T>
T ** qing_clear_2d_array(T ** ptr, int size1, int size2) {
    if(NULL != ptr) {
        for(int i = 0; i < size1; ++i)  delete [] ptr[i];
        delete ptr;
    }
    ptr = NULL;
    return ptr;
}

template <typename T>
T *** qing_new_3d_array(T *** ptr, int size1, int size2, int size3) {
    ptr = new T**[size1];
    for (int i = 0; i < size1; i ++) {
        ptr[i] = new T*[size2];
        for (int j = 0; j < size2; j ++)  ptr[i][j] = new T[size3];
    }
    return ptr;
}

template <typename T>
T *** qing_clear_3d_array(T *** ptr, int size1, int size2, int size3) {
    if(NULL != ptr) {
        for (int i = 0; i < size1; i ++) {
            for (int j = 0; j < size2; j ++) delete[] ptr[i][j];
            delete[] ptr[i];
        }
        delete[] ptr;
    }
    ptr = NULL;
    return ptr;
}

inline void qing_allocf_3(vector<vector<vector<float> > >& array, int n, int r, int c) {
    array.resize(n, vector<vector<float>>(0));
    for(int i = 0; i < n; ++i) {
        array[i].resize(r, vector<float>(0));
        for(int j = 0; j < r; ++j) {
            array[i][j].resize(c, 0.f);}
    }
  //  cout << array.size() << " x " << array[n-1].size() << " x " << array[n-1][r-1].size() << endl;
}

#endif // QING_MEMORY_H
