/** Implementation file for finding map coordinates for an RST transformation
 *
 *  \file ipcv/geometric_transformation/MapGCP.cpp
 *  \author Jacob Stevens (jss8649@rit.edu)
 *  \date 07 09 2019
 */

#include "MapRST.h"
#include <iostream>


using namespace std;

namespace ipcv {
/** Find the map coordinates (map1, map2) for an RST transformation
 *
 *  \param[in] src           source cv::Mat of CV_8UC3
 *  \param[in] angle         rotation angle (CCW) [radians]
 *  \param[in] scale_x       horizontal scale
 *  \param[in] scale_y       vertical scale
 *  \param[in] translation_x horizontal translation [+ to the right]
 *  \param[in] translation_y vertical translation [+ up]
 *  \param[out] map1         cv::Mat of CV_32FC1 (size of the destination map)
 *                           containing the horizontal (x) coordinates at
 *                           which to resample the source data
 *  \param[out] map2         cv::Mat of CV_32FC1 (size of the destination map)
 *                           containing the vertical (y) coordinates at
 *                           which to resample the source data
 */
bool MapRST(const cv::Mat src, const double angle, const double scale_x,
            const double scale_y, const double translation_x,
            const double translation_y, cv::Mat& map1, cv::Mat& map2) {

    // Create the rotation and scale transformation matrix
    cv::Mat transform;
    transform.create(2,2, CV_32FC1);
    transform.at<float>(0,0) = scale_x*cos(angle);
    transform.at<float>(0,1) = scale_y*sin(angle);
    transform.at<float>(1,0) = -scale_x*sin(angle);
    transform.at<float>(1,1) = scale_y*cos(angle);
    // Find the cartesian coordinates of the corners of the source image
    float h = src.rows;
    float w = src.cols;
    float srcCenterY = h/2;
    float srcCenterX = w/2;
    cv::Mat srcCorners;
    srcCorners.create(2,4, CV_32FC1);
    srcCorners.at<float>(0,0) = -srcCenterX;
    srcCorners.at<float>(1,0) = srcCenterY;
    srcCorners.at<float>(0,1) = -srcCenterX;
    srcCorners.at<float>(1,1) = -srcCenterY;
    srcCorners.at<float>(0,2) = srcCenterX;
    srcCorners.at<float>(1,2) = -srcCenterY;
    srcCorners.at<float>(0,3) = srcCenterX;
    srcCorners.at<float>(1,3) = srcCenterY;
    // Find the coordinates of the transformed image's corners
    cv::Mat mapCorners = transform*srcCorners;
    // Find the height and width of the transformed image using it's coordinates
    double minVal;
    double maxVal;
    minMaxLoc(mapCorners.row(0), &minVal, &maxVal);
    int dstWidth = maxVal - minVal;
    minMaxLoc(mapCorners.row(1), &minVal, &maxVal);
    int dstHeight = maxVal - minVal;
    // Create an Nx2 array of coordinate points where N is the number of elements of the transformed image
    cv::Mat UV;
    UV = ipcv::Indices(dstHeight, dstWidth, true);
    // Perform the inverse transformation on the coordinate system
    cv::Mat XY;
    XY = transform.inv()*UV;
    // Seperate the x and y maps and shift by the translation values
    map1 = XY.row(0) - translation_x;
    map2 = XY.row(1) - translation_y;
    // Add back in the shift to remove negative map coordinates
    map1 += srcCenterX;
    map2 += srcCenterY;
    // Shape the maps to be the size of the transformed image
    map1 = map1.reshape(0, dstHeight);
    map2 = map2.reshape(0, dstHeight);
  return true;
}
}
