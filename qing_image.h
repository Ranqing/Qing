#pragma once
#include "qing_common.h"

//Get mask value of (fx, fy) in mask
//DEPTH: depth of mask
inline unsigned char GetMaskValue(const Mat& mask, const float& fy, const float& fx, const int DEPTH)
{
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
inline float Get1DColorValue(const Mat& image, const Mat&mask, const float& fy, const float& fx, const int DEPTH)
{
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

