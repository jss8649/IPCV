/** Implementation file for mapping a source quad on to a target quad
*
*  \file ipcv/geometric_transformation/MapQ2Q.cpp
*  \author Jacob Stevens (jss8649@rit.edu)
*  \date 02 09 2019
*/

#pragma once

#include <opencv2/core.hpp>

namespace ipcv {

/// Available quantization types
enum class RecognitionType {
  spatial,  ///< Uniform quantization
  matched       ///< Improved greyscale quantization
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
bool CharacterRecognition(const cv::Mat& src, std::string& text, cv::Mat& h, const std::vector<cv::Mat>& templates, const std::vector<int>& codes, const double threshold = 0.99, const RecognitionType recognition_type = RecognitionType::spatial);
}
