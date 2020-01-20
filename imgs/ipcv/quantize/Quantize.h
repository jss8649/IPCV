/** Interface file for image quantization
 *
 *  \file ipcv/quantize/Quantize.h
 *  \author <your name> (<your e-mail address>)
 *  \date DD MMM YYYY
 */

#pragma once

#include <opencv2/core.hpp>

namespace ipcv {

/// Available quantization types
enum class QuantizationType {
  uniform,  ///< Uniform quantization
  igs       ///< Improved greyscale quantization
};

/** Perform grey-level quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[in] quantization_type   the quantization method
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 *
 *  \return a boolean indicating that quantization has been carried out
 *          without error
 */
bool Quantize(const cv::Mat& src, const int quantization_levels,
              const QuantizationType quantization_type, cv::Mat& dst);
}
