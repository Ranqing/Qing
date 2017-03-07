#pragma once
#include "qing_common.h"

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

inline bool qing_float_vec_2_uchar_img(const vector<float>& pixels, const int scale, Mat& img) {
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
    uchar * ptr_src = (uchar *)src_msk.ptr<uchar *>(0);
    uchar * ptr_res = (uchar *)res_msk.ptr<uchar *>(0);
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
        cout << "failed to open " << imgname << endl;
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

