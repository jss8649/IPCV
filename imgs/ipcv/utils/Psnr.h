/** Interface file for computing the PSNR between two images
 *
 *  \file ipcv/utils/Psnr.h
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 28 Dec 2018
 */

#pragma once

#include <opencv2/core.hpp>

namespace ipcv {

/** Compute peak signal-to-noise ratio between the provided source images          
 *
 *  \param[in] src1        source cv::Mat of CV_8UC3
 *  \param[in] src2        source cv::Mat of CV_8UC3
 *  \param[in] max_value   maximum possible value data sources may take on
 *
 *  \return                scalar containing ensemble PSNR between sources
 */
double Psnr(const cv::Mat& src1, const cv::Mat& src2, int max_value);
}
