/** Interface file for mapping a source quad on to a target quad
 *
 *  \file ipcv/geometric_transformation/MapQ2Q.h
 *  \author <your name> (<your e-mail address>)
 *  \date DD MMM YYYY
 */

#pragma once

#include <iostream>
#include <opencv2/core.hpp>

using namespace std;

namespace ipcv {

/** Find the source coordinates (map1, map2) for a quad to quad mapping
 *
 *  \param[in] src       source cv::Mat of CV_8UC3
 *  \param[in] tgt       target cv::Mat of CV_8UC3
 *  \param[in] src_vertices
 *                       vertices cv:Point of the source quadrilateral (CW)
 *                       which is to be mapped to the target quadrilateral
 *  \param[in] tgt_vertices
 *                       vertices cv:Point of the target quadrilateral (CW)
 *                       into which the source quadrilateral is to be mapped
 *  \param[out] map1     cv::Mat of CV_32FC1 (size of the destination map)
 *                       containing the horizontal (x) coordinates at
 *                       which to resample the source data
 *  \param[out] map2     cv::Mat of CV_32FC1 (size of the destination map)
 *                       containing the vertical (y) coordinates at
 *                       which to resample the source data
 */
    bool MapRotation3D(const cv::Mat src, cv::Mat& map1, cv::Mat& map2, vector<cv::Point2f>& ptsOut, const double theta=0, const double phi=0, const double psi=0);
}