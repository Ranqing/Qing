#ifndef QING_DISP_H
#define QING_DISP_H

#include <vector>
#include <cmath>
using namespace std;

//离散级数到视差值的换算, d(k) = (dmin*(1-k) + dmax*(k)) / (dmax-dmin)
inline float qing_k_2_disp(const int maxd, const int mind, const int k) {
    return (((mind)*(1-(k)) + (maxd)*(k))/((maxd)- (mind)));
}

//视差值换算到离散级数
inline int qing_disp_2_k(const int maxd, const int mind, const float d) {
    return ((d) - (mind)/((maxd)-(mind))) ;
}

inline float qing_calc_ncc_value(const vector<float>& ncc_vec_l, const vector<float>& ncc_vec_r) {
    if(0==ncc_vec_l.size() || 0==ncc_vec_r.size() || ncc_vec_l.size() != ncc_vec_r.size() ) return 0.f;
    double fenzi = 0.0, fenmu1 = 0.0, fenmu2 = 0.0, fenmu = 0.0;
    for(int i = 0; i < ncc_vec_l.size(); ++i) {
        fenzi += ncc_vec_l[i] * ncc_vec_r[i];
        fenmu1 += ncc_vec_l[i] * ncc_vec_l[i];
        fenmu2 += ncc_vec_r[i] * ncc_vec_r[i];
    }

    fenmu = sqrt(fenmu1) * sqrt(fenmu2);
    if(fenmu == 0.0) return 0.1;
    else
        return fenzi/fenmu;
}

#endif // QING_DISP_H
