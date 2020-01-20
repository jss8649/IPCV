/** Implementation file for image enhancement using linear histogram statistics
 *
 *  \file ipcv/histogram_enhancement/LinearLut.cpp
 *  \author Jake Stevens (jss8649@rit.edu)
 *  \date 09 15 2019
 */

#include "LinearLut.h"

#include <iostream>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Create a 3-channel (color) LUT using linear histogram enhancement
 *
 *  \param[in] src          source cv::Mat of CV_8UC3
 *  \param[in] percentage   the total percentage to remove from the tails
 *                          of the histogram to find the extremes of the
 *                          linear enhancemnt function
 *  \param[out] lut         3-channel look up table in cv::Mat(3, 256)
 */
bool LinearLut(const cv::Mat& src, const int percentage, cv::Mat& lut) {
    // Generate the histogram and cumulative mass function of the source image
    cv::Mat_<int> h;
    ipcv::Histogram(src, h);
    cv::Mat_<double> cdf;
    ipcv::HistogramToCdf(h, cdf);
    lut = cv::Mat::zeros(3, 256, CV_8U);
//    cout << cdf <<endl;
    
    double minVal;
    double maxVal;
    cv::Point minIdx;
    cv::Point maxIdx;
    double value;
    lut.create(3, 256, CV_8UC1);
    // Find the best mapping between the CDFs by finding the index with the minimum difference in
    // values for each channels
//    value = cdf.at<double>(0, dc);
//    cv::minMaxLoc(abs(blue-value), &minVal, &maxVal, &minIdx, &maxIdx);
//    lut.at<uchar>(0, dc) = minIdx.x;
//    value = src_cdf.at<double>(1, dc);
//    cv::minMaxLoc(abs(green-value), &minVal, &maxVal, &minIdx, &maxIdx);
//    lut.at<uchar>(1, dc) = minIdx.x;
//    value = src_cdf.at<double>(2, dc);
//    cv::minMaxLoc(abs(red-value), &minVal, &maxVal, &minIdx, &maxIdx);
//    lut.at<uchar>(2, dc) = minIdx.x;
    // Calculate the upper and lower cutoffs for the histogram
//    double lower 
    double lower = percentage*0.01;
    double upper = 1.-lower;
    // Loop through the source's cumulative mass function
    int low;
    for(int i = 0; i < cdf.rows; i++){
        for (int j = 0; j < cdf.cols; j++) {
            // Leave the values lower than the cutoff at zero
            double cdfVal = cdf.at<double>(i,j);
            if(cdfVal >= lower){
                low = j;
                break;
            }
        }
    }
    int up;
    for(int i = 0; i < cdf.rows; i++){
        for (int j = 0; j < cdf.cols; j++) {
            // Leave the values lower than the cutoff at zero
            double cdfVal = cdf.at<double>(i,j);
            if(cdfVal >= upper){
                up = j;
                break;
            }
        }
    }
    
    for(int i = 0; i < cdf.rows; i++){
        for (int j = 0; j < cdf.cols; j++) {
            // Leave the values lower than the cutoff at zero
            if (j > low) {
                lut.at<uchar>(i,j) = (j-low)*(255/(up-low));
            }
            if (j > up) {
                lut.at<uchar>(i,j) = 255;
            }
            
        }
    }
    
  return true;
}
}
