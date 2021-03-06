#pragma once
#include "qing_common.h"
#include "qing_basic.h"
#include "qing_macros.h"

//template <typename T>
//void qing_gray_to_bgr(T gray, unsigned char * bgr) {
//
//
//}

template<typename T>
inline void qing_img_2_vec(const Mat& img, vector<T>& pixels) {
    int total = img.size().width * img.size().height * img.channels();
    pixels.clear();
    pixels.resize(total);
    memcpy(&pixels.front(), img.data, sizeof(T) * total);
}

template<typename T>
inline bool qing_vec_2_img(const vector<T>& pixels, Mat& img) {
    if(NULL == img.data) {
        cerr << "invalid memory of img...alloc memory first.." << endl;
        return false;
    }

    int total = img.size().width * img.size().height * img.channels();
    if(total != pixels.size()) {
        cerr << "invalid vector size..." << endl;
        return false;
    }

    memcpy(img.data, &pixels.front(), sizeof(T)*total);
    return true;
}

inline Mat qing_uchar_array_2_image(unsigned char * colors, const int h, const int w, const int ch)
{
    Mat image;
    if(1==ch) image.create(h, w, CV_8UC1);
    else if(3==ch) image.create(h,w,CV_8UC3);

    memcpy(image.data, colors, h*w*ch);
    return image;
}

inline bool qing_float_vec_2_uchar_img(const vector<float>& pixels, const float scale, Mat& img) {
    if(NULL == img.data) {
        cerr << "invalid memory of img...alloc memory first.." << endl;
        return false;
    }

    Mat img_mat(img.size(), CV_32FC1);
    qing_vec_2_img(pixels, img_mat);
    img_mat.convertTo(img, CV_8U, scale);
}

inline bool qing_threshold_msk(Mat& src_msk, Mat& dst_msk, const int thresh, const int maxval  ) {
    Mat res_msk = src_msk.clone();
    uchar * ptr_src = (uchar *)src_msk.ptr<uchar>(0);
    uchar * ptr_res = (uchar *)res_msk.ptr<uchar>(0);
    int total = src_msk.size().height * src_msk.size().width;
    for(int i = 0; i < total; ++i) {
        if(ptr_src[i] > (uchar)thresh) ptr_res[i] = (uchar)maxval;
        else ptr_res[i] = 0;
    }
    dst_msk = res_msk.clone();
}

inline bool qing_load_image(const string& imgname, const int mode, Mat& img) {
    img = imread(imgname, mode);
    if(NULL == img.data) {
        cerr << "failed to open " << imgname << endl;
        return false;
    }
    if(CV_LOAD_IMAGE_GRAYSCALE == mode)
    {
        //binarization
        //threshold(img, img, 128, 255, CV_8UC1);
        //threshold(img, img, 128, 255, CV_8UC1);
        qing_threshold_msk(img, img, 128, 255);
        qing_threshold_msk(img, img, 128, 255);
    }
    return true;
}

//load image from std::stream
inline bool qing_load_image(std::istream& in, const int mode, Mat& img) {

    std::vector<char> data;
    in >> std::noskipws;
    std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(), std::back_inserter(data));

    img = imdecode(Mat(data), mode);
    if(0!=img.data)
        return true;
    else
        return false;
}

//inline void qing_save_image(std::ostream& out, const int M, const int N, const int Q) {
//    unsigned char *image = (unsigned char *) new unsigned char [M*N];
//    for(i=0; i<N; i++)
//        for(j=0; j<M; j++)
//            image[i*M+j]=(unsigned char)fimage[i][j];

//    //    out.open(fname, ios::out);
//    //    if (!ofp) {
//    //      cout << "Can't open file: " << fname << endl;
//    //      exit(1);
//    //     }

//    //pgm
//    out << "P5" << endl;
//    out << M << " " << N << endl;
//    out << Q << endl;
//    out.write( reinterpret_cast<char *>(image), (M*N)*sizeof(unsigned char));

//    if (out.fail()) {
//        cout << "Can't write image " << fname << endl;
//        exit(0);
//    }
//    out.close();
//}

//example: ".jpg"
inline void qing_save_image(const string suffix, const Mat& img, vector<unsigned char>& buff, vector<int> param = vector<int>() ) {
    //
    //          vector<uchar> buff;//buffer for coding
    //          vector<int> param = vector<int>(2);
    //          param[0]=CV_IMWRITE_JPEG_QUALITY;
    //          param[1]=95;//default(95) 0-100

    imencode(suffix,img,buff,param);
    cout<<"coded file size(jpg)"<<buff.size()<<endl;//fit buff size automatically.
}


//opencv depth enum { CV_8U=0, CV_8S=1, CV_16U=2, CV_16S=3, CV_32S=4, CV_32F=5, CV_64F=6 };
inline void qing_save_image(const string& imgname, const Mat& img, const int scale = 1) {
    Mat tmp = img.clone();

    if( 0 != img.depth() ) {
        img.convertTo(tmp, CV_8U, scale);
    }
    if( 3 == img.channels() ) {
        cvtColor(tmp, tmp, CV_RGB2BGR);
    }
    imwrite(imgname, tmp);
}

//erode images
inline Mat qing_erode_image(const Mat& image, const int wsize ) {
    Mat result;
    Mat elem = getStructuringElement(MORPH_RECT, cv::Size(wsize, wsize));

    cv::erode(image, result, elem);
    return result;
}

//substract image
inline void qing_subtract_image(Mat& sub_mat, Mat& mat_1, Mat& mat_2) {
    if(mat_1.size() != mat_2.size()) {
        cerr << "different size.." << endl;
        return;
    }

    sub_mat = Mat::zeros(mat_1.size(), mat_1.type());
    unsigned char * ptr_sub = sub_mat.ptr<unsigned char>(0);
    unsigned char * ptr_1 = mat_1.ptr<unsigned char>(0);
    unsigned char * ptr_2 = mat_2.ptr<unsigned char>(0);
    int total_size = mat_1.size().width * mat_1.size().height * mat_1.channels();

    for(int i = 0; i < total_size; ++i) {
        ptr_sub[i] = abs(ptr_1[i] - ptr_2[i]);
    }
}

//shift image
//x' = x + x_offset
//y' = y + y_offset
//y_offset > 0 shift down
//y_offset < 0 shift up
//inline void qing_shift_image(Mat& out_image, Mat& image, int y_offset, int x_offset = 0) {
//    int h = image.size().height;
//    int w = image.size().width;
//    out_image = Mat::zeros(h,w,image.type());

////    if(y_offset > 0) {
////        int crop_x = 0 + x_offset; crop_x = min( max(0, crop_x), w-1);
////        int crop_y = 0 + y_offset; crop_y = min( max(0, crop_y), h-1);
////        int crop_h = h - y_offset; crop_h = min( max(0, crop_h), h);
////        int crop_w = w - x_offset; crop_w = min( max(0, crop_w), w);

////       // cout << crop_x << '\t' << crop_y << '\t' << crop_w << '\t' << crop_h << endl;
////        image(Rect(crop_x, crop_y, crop_w, crop_h)).copyTo(out_image(Rect(0,0,crop_w,crop_h)));
////    }
////    else if(y_offset < 0) {
////        int crop_x = 0;
////        int crop_y = 0;
////        int crop_h = h + y_offset;
////        int crop_w = w + x_offset;

////       // cout << crop_x << '\t' << crop_y << '\t' << crop_w << '\t' << crop_h << endl;
////        image(Rect(crop_x, crop_y, crop_w, crop_h)).copyTo(out_image(Rect(abs(x_offset), abs(y_offset), crop_w, crop_h)));
////    }

//    if(y_offset > 0) { //down
//        int crop_h = h - y_offset;
//        int crop_w = w - x_offset;
//        int crop_x = 0;
//        int crop_y = 0;

//        cout << "shift image down in y\t" << y_offset  << '\t' << crop_h << " x " << crop_w << endl;
//        image(Rect(crop_x, crop_y, crop_w, crop_h)).copyTo(out_image(Rect(x_offset, y_offset, crop_w, crop_h)));
//    }
//    else if(y_offset < 0) {
//        int crop_h = h + y_offset;
//        int crop_w = w - x_offset;
//        int crop_x = abs(x_offset);
//        int crop_y = abs(y_offset);

//        cout << "shift image up in y\t" << y_offset << '\t' << crop_h << " x " << crop_w << endl;
//        image(Rect(crop_x, crop_y, crop_w, crop_h)).copyTo(out_image(Rect(0,0,crop_w, crop_h)));
//    }
//}

inline Mat qing_shift_image(Mat& image, int y_offset, int x_offset = 0) {
    int h = image.size().height;
    int w = image.size().width;
    Mat out_image = Mat::zeros(h,w,image.type());

    if(y_offset > 0) { //down
        int crop_h = h - y_offset;
        int crop_w = w - x_offset;
        int crop_x = 0;
        int crop_y = 0;

        cout << "shift image down in y\t" << y_offset  << '\t' << crop_h << " x " << crop_w << endl;
        image(Rect(crop_x, crop_y, crop_w, crop_h)).copyTo(out_image(Rect(x_offset, y_offset, crop_w, crop_h)));
    }
    else if(y_offset < 0) {
        int crop_h = h + y_offset;
        int crop_w = w - x_offset;
        int crop_x = abs(x_offset);
        int crop_y = abs(y_offset);

        cout << "shift image up in y\t" << y_offset << '\t' << crop_h << " x " << crop_w << endl;
        image(Rect(crop_x, crop_y, crop_w, crop_h)).copyTo(out_image(Rect(0,0,crop_w, crop_h)));
    }
    return out_image;
}

//x_offset > 0 right;
//x_offset < 0 left;
//template<typename T>
inline void qing_shift_image_x(Mat& shifted, Mat& image, int x_offset, int border_type) {
    shifted = Mat::zeros(image.size(), image.type());
    int h = image.size().height;
    int w = image.size().width;
    int abs_x_offset = abs(x_offset);

    if(x_offset > 0) {
        image(Rect(0,0,w-abs_x_offset,h)).copyTo(shifted(Rect(abs_x_offset,0,w-abs_x_offset,h)));
        switch(border_type) {
        case QING_DUPLICATE_BORDER: {
            for(int x = 0; x < abs_x_offset; ++x)
                image.col(0).copyTo(shifted.col(x));
            break;
        }
        default: break;
        }
    }
    else {
        image(Rect(abs_x_offset, 0, w-abs_x_offset, h)).copyTo(shifted(Rect(0,0,w-abs_x_offset, h)));
        switch(border_type) {
        case QING_DUPLICATE_BORDER: {
            for(int x = w-abs_x_offset; x < w; ++x)
                image.col(w-1).copyTo(shifted.col(x));
            break;
        }
        default: break;
        }
    }
}

template<typename T>
inline void qing_map_jet(T v, T vmin, T vmax, unsigned char& r, unsigned char& g, unsigned char& b)
{
        r = 255;
        g = 255;
        b = 255;

        if (v < vmin) {
            v = vmin;
        }

        if (v > vmax) {
            v = vmax;
        }

        double dr, dg, db;

        if (v < 0.1242) {
            db = 0.504 + ((1.-0.504) / 0.1242)*v;
            dg = dr = 0.;
        } else if (v < 0.3747) {
            db = 1.;
            dr = 0.;
            dg = (v - 0.1242) * (1. / (0.3747-0.1242));
        } else if (v < 0.6253) {
            db = (0.6253 - v) * (1. / (0.6253-0.3747));
            dg = 1.;
            dr = (v - 0.3747) * (1. / (0.6253-0.3747));
        } else if (v < 0.8758) {
            db = 0.;
            dr = 1.;
            dg = (0.8758 - v) * (1. / (0.8758-0.6253));
        } else {
            db = 0.;
            dg = 0.;
            dr = 1. - (v - 0.8758) * ((1.-0.504) / (1.-0.8758));
        }

        r = (unsigned char) (255 * dr);
        g = (unsigned char) (255 * dg);
        b = (unsigned char) (255 * db);
}

//src: input img
//dst: result img that applying color jet map on input img
//noval: pixels of which values equal noval will be omited
inline void qing_apply_colormap_jet(Mat& out, vector<float>& src, int len, float maxval, float noval) {
    vector<float> t_src(len);
    copy(src.begin(), src.end(), t_src.begin());
    qing_vec_normalize<float>(t_src, maxval);

    int h = out.size().height;
    int w = out.size().width;
    out = Mat::zeros(out.size(), CV_8UC3);
    unsigned char * ptr_out = out.ptr<unsigned char>(0), b, g, r;

    for(int i = 0; i < len; ++i) {
        if(noval == t_src[i]) continue;
        qing_map_jet<float>(t_src[i], 0.f, 1.f, r, g, b);
        ptr_out[i*3+0] = b;
        ptr_out[i*3+1] = g;
        ptr_out[i*3+2] = r;
    }

//    imshow("colormap_jet", out);
//    waitKey(0);
//    destroyWindow("colormap_jet");
}

//Get mask value of (fx, fy) in mask
//DEPTH: depth of mask
inline unsigned char GetMaskValue(const Mat& mask, const float& fy, const float& fx, const int DEPTH) {
    CV_Assert(DEPTH == CV_8U && mask.channels() == 1);

    int w = mask.size().width;
    int h = mask.size().height;

    //bilinear case
    const int ix = (int)floor(fx);
    const int iy = (int)floor(fy);

    const float dx1 = fx - ix;    const float dx0 = 1.f - dx1;
    const float dy1 = fy - iy;    const float dy0 = 1.f - dy1;
    const float f00 = dx0 * dy0;  const float f01 = dx0 * dy1;
    const float f10 = dx1 * dy0;  const float f11 = dx1 * dy1;

    const unsigned char value1 = mask.at<unsigned char>(iy, ix);
    const unsigned char value2 = mask.at<unsigned char>(iy, min( (ix+1),(w-1)) );
    const unsigned char value3 = mask.at<unsigned char>(min( (iy+1), (h-1)), ix);
    const unsigned char value4 = mask.at<unsigned char>(min( (iy+1), (h-1)), min((ix+1), (w-1)) );

    float value = f00 * value1 + f10 * value2 + f01 * value3 + f11 * value4;
    return (value > 125.f) ? 255 : 0;
}

//Get 1-channel color value of (fx, fy) in image
//DEPTH: depth of image
inline float Get1DColorValue(const Mat& image, const Mat&mask, const float& fy, const float& fx, const int DEPTH) {
    CV_Assert(image.channels() == 1);

    int w = mask.size().width;
    int h = mask.size().height;

    //bilinear case
    const int ix = (int)floor(fx);
    const int iy = (int)floor(fy);

    const float dx1 = fx - ix;    const float dx0 = 1.f - dx1;
    const float dy1 = fy - iy;    const float dy0 = 1.f - dy1;
    const float f00 = dx0 * dy0;  const float f01 = dx0 * dy1;
    const float f10 = dx1 * dy0;  const float f11 = dx1 * dy1;

    float value1 = 0.0f, value2 = 0.0f, value3 = 0.0f, value4 = 0.0f;

    if(DEPTH == CV_32F)
    {
        value1 = image.at<float>(iy, ix);                           //(x,y)
        value2 = image.at<float>(iy, min(ix+1, w-1));               //(x+1,y)
        value3 = image.at<float>(min(iy+1, h-1), ix);               //(x,y+1)
        value4 = image.at<float>(min(iy+1, h-1), min(ix+1, w-1));   //(x+1,y+1)
    }
    else if (DEPTH == CV_64F)
    {
        value1 = image.at<double>(iy, ix);                           //(x,y)
        value2 = image.at<double>(iy, min(ix+1, w-1));               //(x+1,y)
        value3 = image.at<double>(min(iy+1, h-1), ix);               //(x,y+1)
        value4 = image.at<double>(min(iy+1, h-1), min(ix+1, w-1));   //(x+1,y+1)
    }

    float value = f00 * value1 + f10 * value2 + f01 * value3 + f11 * value4;
    return value;
}

