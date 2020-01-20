/** Implementation file for finding source image coordinates for a source-to-map
 *  remapping using ground control points
 *
 *  \file ipcv/geometric_transformation/MapGCP.cpp
 *  \author Jacob Stevens (jss8649@rit.edu)
 *  \date 07 09 2019
 */

#include "MapGCP.h"

#include <iostream>
//#include "imgs/ipcv/utils/Utils.h"
#include <opencv2/core.hpp>

using namespace std;

namespace ipcv {

/** Find the source coordinates (map1, map2) for a ground control point
 *  derived mapping polynomial transformation
 *
 *  \param[in] src   source cv::Mat of CV_8UC3
 *  \param[in] map   map (target) cv::Mat of CV_8UC3
 *  \param[in] src_points
 *                   vector of cv::Points representing the ground control
 *                   points from the source image
 *  \param[in] map_points
 *                   vector of cv::Points representing the ground control
 *                   points from the map image
 *  \param[in] order  mapping polynomial order
 *                      EXAMPLES:
 *                        order = 1
 *                          a0*x^0*y^0 + a1*x^1*y^0 +
 *                          a2*x^0*y^1
 *                        order = 2
 *                          a0*x^0*y^0 + a1*x^1*y^0 + a2*x^2*y^0 +
 *                          a3*x^0*y^1 + a4*x^1*y^1 +
 *                          a5*x^0*y^2
 *                        order = 3
 *                          a0*x^0*y^0 + a1*x^1*y^0 + a2*x^2*y^0 + a3*x^3*y^0 +
 *                          a4*x^0*y^1 + a5*x^1*y^1 + a6*x^2*y^1 +
 *                          a7*x^0*y^2 + a8*x^1*y^2 +
 *                          a9*x^0*y^3
 *  \param[out] map1  cv::Mat of CV_32FC1 (size of the destination map)
 *                    containing the horizontal (x) coordinates at which to
 *                    resample the source data
 *  \param[out] map2  cv::Mat of CV_32FC1 (size of the destination map)
 *                    containing the vertical (y) coordinates at which to
 *                    resample the source data
 */
bool MapGCP(const cv::Mat src, const cv::Mat map,
            const vector<cv::Point> src_points,
            const vector<cv::Point> map_points, const int order,
            cv::Mat& map1, cv::Mat& map2) {

    // Format the ground control points(GCP) into 1D matrices
    cv::Mat mapx, mapy, srcx, srcy;
    mapx.create(map_points.size(), 1, CV_32F);
    mapy.create(map_points.size(), 1, CV_32F);
    srcx.create(src_points.size(), 1, CV_32F);
    srcy.create(src_points.size(), 1, CV_32F);
    for (int i = 0; i < map_points.size(); i++){
        mapx.at<float>(i) = map_points[i].x;
        mapy.at<float>(i) = map_points[i].y;
        srcx.at<float>(i) = src_points[i].x;
        srcy.at<float>(i) = src_points[i].y;
    }

    // Create a 2xN matrix of the cartesian coordinates of the map image
    // where N is the number of points(pixels locations) in the map
//    cv::Mat uv = ipcv::Indices(map.rows, map.cols, false);
    cv::Mat uv;
    int index = 0;
    uv = cv::Mat::zeros(2, map.rows*map.cols, CV_32FC1);
    for (int i = 0; i<map.rows; i++){
      for (int j = 0; j<map.cols; j++){
          uv.ptr<float>(0)[index] = j;
          uv.ptr<float>(1)[index] = i;
          index++;
        }
    }
    cv::Mat u = uv.row(0).t();
    cv::Mat v = uv.row(1).t();
    
    cv::Mat polyMat;
    cv::Mat xyCol;
    cv::Mat xCol;
    cv::Mat yCol;
    cv::Mat uCol;
    cv::Mat vCol;
    cv::Mat uvCol;
    cv::Mat uvPolyMat;
    // Create the polynomial matrices:
    // polyMat is the polynomial matrix created from the GCP
    // uvPolyMat is the polynomial matrix created from the entire map image
    for (int yOrder = 0; yOrder <= order; yOrder++){
        for (int xOrder = 0; xOrder <= order; xOrder++){
            cv::pow(mapx, xOrder, xCol);
            cv::pow(mapy, yOrder, yCol);
            cv::pow(u, xOrder, uCol);
            cv::pow(v, yOrder, vCol);
            if (xOrder == 0 && yOrder == 0){
                cv::multiply(xCol, yCol, polyMat, 1, CV_32F);
                cv::multiply(uCol, vCol, uvPolyMat, 1, CV_32F);
            }
            cv::multiply(xCol, yCol, xyCol, 1, CV_32F);
            cv::hconcat(polyMat, xyCol, polyMat);
            cv::multiply(uCol, vCol, uvCol, 1, CV_32F);
            cv::hconcat(uvPolyMat, uvCol, uvPolyMat);
        }
    }
    // Create the least squares solution matrices
    cv::Mat map1Coeffs = polyMat.inv(cv::DECOMP_SVD) * srcx;
    cv::Mat map2Coeffs = polyMat.inv(cv::DECOMP_SVD) * srcy;
    // Run the map image's polynomial mat through the least squares solution
    // to find the corresponding points in the orginal image
    map1 = uvPolyMat * map1Coeffs;
    map2 = uvPolyMat * map2Coeffs;
    // Shape the maps to be the size of the transformed image
    map1 = map1.reshape(0, map.rows);
    map2 = map2.reshape(0, map.rows);
    return true;
}
}
