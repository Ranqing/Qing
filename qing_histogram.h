#pragma once
#include "qing_common.h"

inline void checkHistogram(cv::Mat image, cv::Mat mask, const string savefn = "")
{
    vector<cv::Mat> bgrPlanes;
    split(image, bgrPlanes);

    int histSize = 256;
    float range[] = { 0, histSize*1.0f};
    const float * histRange = { range };

    bool uniform = true;
    bool accumulate = false;

    Mat bHist, gHist, rHist;
    calcHist(&bgrPlanes[0], 1, 0, mask, bHist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgrPlanes[1], 1, 0, mask, gHist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgrPlanes[2], 1, 0, mask, rHist, 1, &histSize, &histRange, uniform, accumulate);

    int histW = 512;
    int histH = 400;
    int binW = cvRound((double)histW/histSize);

    Mat histImage(histH, histW, CV_8UC3, Scalar(0,0,0));

    normalize(bHist, bHist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(gHist, gHist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(rHist, rHist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    for(int i = 1; i < histSize; i++)
    {
        line(histImage, Point(binW * (i-1), histH - cvRound(bHist.at<float>(i-1)) ),
                        Point(binW * (i), histH - cvRound(bHist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8, 0);    //b
        line(histImage, Point(binW * (i-1), histH - cvRound(gHist.at<float>(i-1)) ),
                        Point(binW * (i), histH - cvRound(gHist.at<float>(i))),
             Scalar(0, 255, 0), 2, 8, 0);    //g
        line(histImage, Point(binW * (i-1), histH - cvRound(rHist.at<float>(i-1)) ),
                        Point(binW * (i-1), histH - cvRound(rHist.at<float>(i))),
             Scalar(0, 0, 255), 2, 8, 0);   //r
    }

    if(savefn != "")
    {
        imwrite(savefn, histImage);
    }
    else
    {
        namedWindow("checkHist", CV_WINDOW_AUTOSIZE);
        imshow("checkHist", histImage);
        waitKey(0);
        destroyWindow("checkHist");
    }
}
