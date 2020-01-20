/** Interface file for the creation of a coordinate function
 *
 *  \file ipcv/utils/Indices.h
 *  \author Jake Stevens (jss8649@rit.edu)
 *  \date 04 Aug 2019
 */

#pragma once

#include <opencv2/core.hpp>

namespace ipcv {

/** Finds the 2D coordinate locations of a matrix
 *
 *  \param[in] rows   number of rows
 *  \param[in] cols   number of columns
 *
 */
cv::Mat Indices(const int rows, const int cols, const bool shift);
}
