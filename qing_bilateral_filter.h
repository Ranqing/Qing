#ifndef QING_BILATERAL_FILTER_H
#define QING_BILATERAL_FILTER_H

//bilateral_filter_one_channel
inline void qing_bilateral_filter_1ch(const vector<float>& img, const vector<uchar>& msk, const int w, const int h, vector<float>& cost_vol,
                      const int wnd = 9,  double sig_dis = 4.5, double sig_color = 0.03 ) {
    sig_dis = wnd * 0.5;
    int offset = (int)(sig_dis);

    vector<float> dest(w*h, 0.f);

    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            int cen_idx = y * w + x;

            if( 0==msk[cen_idx] ) continue;

            double sum = 0., sum_weight = 0.;
            for(int dy = -offset; dy <= offset; ++dy)
            {
                int cury = (y + dy + h) % h;
                for(int dx = -offset; dx <= offset; ++dx)
                {
                    int curx = (x + dx + w) % w;
                    int cur_idx = cury * w + curx;

                    if( 0==msk[cur_idx] ) continue;

                    double delta_dis_2 = dx * dx + dy * dy;
                    double delta_color_2 = fabs( img[cur_idx] - img[cen_idx] );
                    double weight = exp( -delta_dis_2 / (sig_dis*sig_dis) - delta_color_2 / (sig_color * sig_color) );

                    sum += weight * cost_vol[cur_idx];
                    sum_weight += weight;
                }
            }

            dest[cen_idx] = sum/sum_weight;
        }
    }
    copy(dest.begin(), dest.end(), cost_vol.begin());
}

#endif // QING_BILATERAL_FILTER_H
