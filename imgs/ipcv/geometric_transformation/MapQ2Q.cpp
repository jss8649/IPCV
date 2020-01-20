/** Implementation file for mapping a source quad on to a target quad
 *
 *  \file ipcv/geometric_transformation/MapQ2Q.cpp
 *  \author Jacob Stevens (jss8649@rit.edu)
 *  \date 02 09 2019
 */

#include "MapQ2Q.h"

#include <iostream>
#include <opencv2/core.hpp>
#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Find the source coordinates (map1, map2) for a quad to quad mapping
 * Note: this function is the same as opencv's getPerspectiveTransform but provides x and y maps instead of a projection matrix
 *
 *  \param[in] src       source cv::Mat of CV_8UC3
 *  \param[in] tgt       target cv::Mat of CV_8UC3
 *  \param[in] src_vertices
 *                       vertices cv::Point of the source quadrilateral (CW)
 *                       which is to be mapped to the target quadrilateral
 *  \param[in] tgt_vertices
 *                       vertices cv::Point of the target quadrilateral (CW)
 *                       into which the source quadrilateral is to be mapped
 *  \param[out] map1     cv::Mat of CV_32FC1 (size of the destination map)
 *                       containing the horizontal (x) coordinates at
 *                       which to resample the source data
 *  \param[out] map2     cv::Mat of CV_32FC1 (size of the destination map)
 *                       containing the vertical (y) coordinates at
 *                       which to resample the source data
 */
bool MapQ2Q(const cv::Mat src, const cv::Mat tgt,
            const vector<cv::Point2f> src_vertices,
            const vector<cv::Point2f> tgt_vertices, cv::Mat& map1,
            cv::Mat& map2) {

    cv::Mat transMat;
    transMat = cv::Mat::zeros(8, 8, CV_32F);
    cv::Mat srcxy;
    srcxy.create(8, 1, CV_32F);
    // Create the transformation matrix
    for (int i = 0; i < transMat.rows; i++){
        float* ptr = transMat.ptr<float>(i);
        if (i<4){
            ptr[0] = tgt_vertices[i].x;
            ptr[1] = tgt_vertices[i].y;
            ptr[2] = 1.;
            ptr[6] = -tgt_vertices[i].x*src_vertices[i].x;
            ptr[7] = -tgt_vertices[i].y*src_vertices[i].x;
            // Also create an mat of the src points
            srcxy.at<float>(i) = src_vertices[i].x;
        }
        else{
            ptr[3] = tgt_vertices[i-4].x;
            ptr[4] = tgt_vertices[i-4].y;
            ptr[5] = 1.;
            ptr[6] = -tgt_vertices[i-4].x*src_vertices[i-4].y;
            ptr[7] = -tgt_vertices[i-4].y*src_vertices[i-4].y;
            
            srcxy.at<float>(i) = src_vertices[i-4].y;
        }
    }
    // Invert the transformation matrix
    cv::Mat invTransMat = transMat.inv(cv::DECOMP_SVD);
    // Create and shape the projection matrix
    cv::Mat projMat = invTransMat*srcxy;
    cv::Mat ones;
    ones = cv::Mat::ones(1, 1, CV_32F);
    cv::vconcat(projMat, ones, projMat);
    projMat = projMat.reshape(0, 3);
    // Create a 2xN matrix of the cartesian coordinates of the map image
    // where N is the number of points(pixels locations) in the map
    cv::Mat uv = ipcv::Indices(tgt.rows, tgt.cols, false);
    // Add a row of 1 to the coordanate mat to represent the projections scale
    ones = cv::Mat::ones(1, uv.cols, CV_32F);
    cv::vconcat(uv, ones, uv);
    // Perform the projection
    cv::Mat xyh = projMat*uv;
    map1.create(1, xyh.cols, CV_32F);
    map2.create(1, xyh.cols, CV_32F);
    // Divide elementwise by the scalar (h) to find the coordinates of the src image that correspond to the tgt image
    cv::divide(xyh.row(0), xyh.row(2), map1);
    cv::divide(xyh.row(1), xyh.row(2), map2);
    
    // Shape the coordinate map to the size of the map image
    map1 = map1.reshape(0, tgt.rows);
    map2 = map2.reshape(0, tgt.rows);
  return true;
}
}
