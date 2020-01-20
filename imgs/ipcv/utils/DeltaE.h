/** Interface file for computing the delta E between two 3-channel images
 *
 *  \file ipcv/utils/DeltaE.h
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 04 Jan 2019
 */

#pragma once

#include <string>

#include <opencv2/core.hpp>

namespace ipcv {

/** Compute delta E between the provided 3-channel source images
 *
 *  \param[in] src1         source cv::Mat of CV_8UC3
 *  \param[in] src2         source cv::Mat of CV_8UC3
 *  \param[out] dE          destination cv::Mat of CV_32FC1 for delta E
 *  \param[in] max_value    maximum possible value data sources may take on
 *  \param[in] standard     Standard to use 1976 | 1994 | 2000
 *                          [default is 1976]
 *  \param[in] application  Application type to use if the delta E 1994
 *                          standard is selected graphic_arts | textiles
 *                          (ignored otherwise)
 *                          [default is graphic_arts]
 *
 *  \return                 scalar containing the delta E between sources
 */
double DeltaE(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dE,
              int max_value, int standard = 1976,
              std::string application = "graphic_arts");
}
