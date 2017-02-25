#ifndef QING_FILE_READER
#define QING_FILE_READER

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

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
    ifstream fin(filename, ios::in|ios::binary);
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

#endif // QING_FILE_READER

