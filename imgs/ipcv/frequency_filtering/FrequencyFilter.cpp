/** Implementation file for image filtering
 *
 *  \file ipcv/spatial_filtering/Filter2D.cpp
 *  \author <your name> (<your e-mail address>)
 *  \date DD MMM YYYY
 */

#include "FrequencyFilter.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Correlates an image with the provided kernel
 *
 *  \param[in] src          source cv::Mat of CV_8UC3
 *  \param[out] dst         destination cv::Mat of ddepth type
 *  \param[in] ddepth       desired depth of the destination image
 *  \param[in] kernel       convolution kernel (or rather a correlation
 *                          kernel), a single-channel floating point matrix
 *  \param[in] anchor       anchor of the kernel that indicates the relative
 *                          position of a filtered point within the kernel;
 *                          the anchor should lie within the kernel; default
 *                          value (-1,-1) means that the anchor is at the
 *                          kernel center
 *  \param[in] delta        optional value added to the filtered pixels
 *                          before storing them in dst
 *  \param[in] border_mode  pixel extrapolation method
 *  \param[in] border_value value to use for constant border mode
 */
bool FrequencyFilter(const cv::Mat& src, cv::Mat& dst, const int ddepth, const FilterType filter_type, const int cutoffFrequency, const int order, const FilterShape filter_shape, const int delta){
    cv::Mat filter;
    filter = ipcv::Dist(src.rows, src.cols, true);
    if (filter_shape == ipcv::FilterShape::IDEAL) {
        cv::threshold(filter, filter, cutoffFrequency, 1, cv::THRESH_BINARY_INV);
        if (filter_type == ipcv::FilterType::HIGHPASS) {
            filter = 1-filter;
        }
    }
    else if (filter_shape == ipcv::FilterShape::BUTTERWORTH){
        filter /= cutoffFrequency;
        cv::pow(filter, 2*order, filter);
        filter += 1;
        cv::sqrt(filter, filter);
        cv::divide(1, filter, filter);
        if (filter_type == ipcv::FilterType::HIGHPASS) {
            double minVal, maxVal;
            cv::minMaxLoc(filter, &minVal, &maxVal);
            filter = maxVal-filter;
        }
    }
    else if (filter_shape == ipcv::FilterShape::GAUSSIAN){
        filter /= cutoffFrequency;
        cv::pow(filter, 2*order, filter);
        filter *= -0.5;
        cv::exp(filter, filter);
        if (filter_type == ipcv::FilterType::HIGHPASS) {
            double minVal, maxVal;
            cv::minMaxLoc(filter, &minVal, &maxVal);
            filter = maxVal-filter;
        }
    }
    cv::Mat freqSrc;
    src.convertTo(freqSrc, CV_64F);
    cv::dft(freqSrc, freqSrc);
    cv::multiply(freqSrc, filter, freqSrc);
    cv::dft(freqSrc, dst, cv::DFT_INVERSE + cv::DFT_SCALE);
    dst.convertTo(dst, ddepth);
    dst += delta;

return true;
}
}
