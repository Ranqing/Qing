#ifndef QING_FILE_WRITER
#define QING_FILE_WRITER

#include <iostream>
#include <fstream>
using namespace std;


//total_size: length of data
//step_size:  length of output '\n'
inline void qing_write_txt(const string filename, const float* data, const int total_size, const int step_size = 0) {
    ofstream fout(filename, ios_base::out) ;
    if(false == fout.is_open()) {
        cerr << "failed to open " << filename << endl;
        return ;
    }

    int n_steps = 0;
    for(int i = 0, step_count = 0; i < total_size; ++i) {
        fout << data[i] << ' ';
        step_count ++;
        if( step_size == step_count ) {
            fout << endl;
            step_count = 0;
            n_steps ++;
        }
    }
    fout.close();

    cout << "end of saving " << filename << ", total_size = " << total_size << ", step_size = " << step_size << ", " << n_steps << " in total.." << endl;
}

#endif // QING_FILE_WRITER

