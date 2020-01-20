/** Interface file for computing the RMSE between two images
 *
 *  \file ipcv/utils/Rmse.h
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 03 Jan 2019
 */

#pragma once

#include <opencv2/core.hpp>

namespace ipcv {

/** Compute root mean squared error between the provided source images
 *
 *  \param[in] src1        source cv::Mat of CV_8UC3
 *  \param[in] src2        source cv::Mat of CV_8UC3
 *  \param[in] max_value   maximum possible value data sources may take on
 *
 *  \return                scalar containing ensemble RMSE between sources
 */
double Rmse(const cv::Mat& src1, const cv::Mat& src2);
}
