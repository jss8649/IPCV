/** Implementation file for image enhancement using histogram matching
 *
 *  \file ipcv/histogram_enhancement/MatchingLut.cpp
 *  \author Jake Stevens (jss8649@rit.edu)
 *  \date 09 15 2019
 */

#include "MatchingLut.h"

#include <iostream>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Create a 3-channel (color) LUT using histogram matching
 *
 *  \param[in] src   source cv::Mat of CV_8UC3
 *  \param[in] h     the histogram in cv:Mat(3, 256) that the
 *                   source is to be matched to
 *  \param[out] lut  3-channel look up table in cv::Mat(3, 256)
 */
bool MatchingLut(const cv::Mat& src, const cv::Mat& h, cv::Mat& lut) {
  
    // Create a histogram of the source image
    cv::Mat_<int> src_h;
    ipcv::Histogram(src, src_h);
    // Generate the cumulative density function of the source image
    cv::Mat_<double> src_cdf;
    ipcv::HistogramToCdf(src_h, src_cdf);
    // Generate the cumulative density function of the target image
    cv::Mat_<double> tgt_cdf;
    ipcv::HistogramToCdf(h, tgt_cdf);
    // Separate the target cdf rows by color
    cv::Mat blue = tgt_cdf.row(0);
    cv::Mat green = tgt_cdf.row(1);
    cv::Mat red = tgt_cdf.row(2);
    double minVal;
    double maxVal;
    cv::Point minIdx;
    cv::Point maxIdx;
    double value;
    lut.create(3, 256, CV_8UC1);
    for(int dc = 0; dc < lut.cols; dc++){
        // Find the best mapping between the CDFs by finding the index with the minimum difference in
        // values for each channels
        value = src_cdf.at<double>(0, dc);
        cv::minMaxLoc(abs(blue-value), &minVal, &maxVal, &minIdx, &maxIdx);
        lut.at<uchar>(0, dc) = minIdx.x;
        value = src_cdf.at<double>(1, dc);
        cv::minMaxLoc(abs(green-value), &minVal, &maxVal, &minIdx, &maxIdx);
        lut.at<uchar>(1, dc) = minIdx.x;
        value = src_cdf.at<double>(2, dc);
        cv::minMaxLoc(abs(red-value), &minVal, &maxVal, &minIdx, &maxIdx);
        lut.at<uchar>(2, dc) = minIdx.x;
    }

  return true;
}
}
