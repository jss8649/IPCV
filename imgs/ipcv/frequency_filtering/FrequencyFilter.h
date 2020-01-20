/** Interface file for image filtering
 *
 *  \file ipcv/spatial_filtering/Filter2D.h
 *  \author <your name> (<your e-mail address>)
 *  \date DD MMM YYYY
 */

#pragma once

#include <opencv2/core.hpp>

namespace ipcv {

// Available filter types
enum class FilterType {
  LOWPASS,
  HIGHPASS
};
    
// Available filter shapes
enum class FilterShape {
    IDEAL,
    BUTTERWORTH,
    GAUSSIAN
};

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
    bool FrequencyFilter(const cv::Mat& src, cv::Mat& dst, const int ddepth, const FilterType filter_type = ipcv::FilterType::LOWPASS, const int cutoffFrequency = 16, const int order = 1, const FilterShape filter_shape = ipcv::FilterShape::IDEAL, const int delta = 0);
}
