/** Interface file for bilateral filtering
 *
 *  \file ipcv/bilateral_filtering/BilateralFilter.h
 *  \author <your name> (<your e-mail address>)
 *  \date DD MMMM YYYY
 */

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "ipcv"


namespace ipcv {

// Available border modes
enum class BorderMode {
  CONSTANT,  // Use background color
  REPLICATE  // Replicate border pixels
};

/** Bilateral filter an image
 *
 *  \param[in] src             source cv::Mat of CV_8UC3
 *  \param[out] dst            destination cv::Mat of ddepth type
 *  \param[in] sigma_distance  standard deviation of distance/closeness filter
 *  \param[in] sigma_range     standard deviation of range/similarity filter
 *  \param[in] radius          radius of the bilateral filter (if negative, use
 *                             twice the standard deviation of the distance/
 *                             closeness filter)
 *  \param[in] border_mode     pixel extrapolation method
 *  \param[in] border_value    value to use for constant border mode
 */
bool BilateralFilter(const cv::Mat& src, cv::Mat& dst,
                     const double sigma_distance, const double sigma_range,
                     const int radius,
                     const BorderMode border_mode = BorderMode::REPLICATE,
                     uint8_t border_value = 0);
}
