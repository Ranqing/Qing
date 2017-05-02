#ifndef QING_MIDDLEBURY_H
#define QING_MIDDLEBURY_H

#include <iostream>
#include <string>
using namespace std;

#include "pfm.h"

class qing_middlebury_calib_info {
public:
    qing_middlebury_calib_info()  {}
    ~qing_middlebury_calib_info() {}

private:
    float m_cam0[9], m_cam1[9];
    float m_doffs, m_baseline;
    int m_width, m_height, m_ndisp;

public:
    //need to be re-written
    void read(const string filename) {
        fstream fin(filename, ios::in);
        if(fin.is_open() == false) {
            cerr << "failed to open " << filename << endl;
            return ;
        }

        for(int i = 0; i < 9; ++i) fin >> m_cam0[i];
        for(int i = 0; i < 9; ++i) fin >> m_cam1[i];
        fin >> m_doffs;
        fin >> m_baseline;
        fin >> m_width;
        fin >> m_height;
        fin >> m_ndisp;
        fin.close();

# if 0
        cout << "open "  << filename << endl;
        cout << "cam0: " << endl;
        for(int i = 0; i < 9; ++i) cout << m_cam0[i] << ' ';
        cout << endl << "cam1: " << endl;
        for(int i = 0; i < 9; ++i) cout << m_cam1[i] << ' ';
        cout << endl << "d offset = " << m_doffs;
        cout << endl << "basline = " << m_baseline;
        cout << endl << m_width << " x " << m_height << ", " << m_ndisp << endl;
# endif
    }

    float * get_cam0() { float * cam0 = &m_cam0[0]; return cam0; }
    float * get_cam1() { float * cam1 = &m_cam1[0]; return cam1; }
    float get_d_offset()  { return m_doffs; }
    float get_baseline()  { return m_baseline; }
    int get_width()       { return m_width;    }
    int get_height()      { return m_height;   }
    int get_ndisp()       { return m_ndisp;    }

};

class qing_pfm_io {

};

#endif // QING_MIDDLEBURY_H
