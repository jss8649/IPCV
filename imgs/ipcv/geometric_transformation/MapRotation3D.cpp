/** Implementation file for mapping a source quad on to a target quad
 *
 *  \file ipcv/geometric_transformation/MapQ2Q.cpp
 *  \author <your name> (<your e-mail address>)
 *  \date DD MMM YYYY
 */

#include "MapRotation3D.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "MapQ2Q.h"
#include "imgs/ipcv/utils/Utils.h"
#include <math.h>

using namespace std;

namespace ipcv {

/** Find the source coordinates (map1, map2) for a quad to quad mapping
 *
 *  \param[in] src       source cv::Mat of CV_8UC3
 *  \param[in] ptsOut         vertices cv::Point of the target quadrilateral (CW)
 *                       into which the source quadrilateral is to be mapped
*                       \param[in] phi        angle in radians to rotate the source data along the x-axis
*                       \param[in] psi        angle in radians to rotate the source data along the y-axis
*                       \param[in] theta        angle in radians to rotate the source data along the z-axis
 *  \param[out] map1     cv::Mat of CV_32FC1 (size of the destination map)
 *                       containing the horizontal (x) coordinates at
 *                       which to resample the source data
 *  \param[out] map2     cv::Mat of CV_32FC1 (size of the destination map)
 *                       containing the vertical (y) coordinates at
 *                       which to resample the source data
 */
bool MapRotation3D(const cv::Mat src, cv::Mat& map1, cv::Mat& map2, vector<cv::Point2f>& ptsOut, const double theta, const double phi, const double psi) {
    // Define field of view
    double fov = 60;
    double halfFovy=fov*0.5;
    double radians_per_degree = 3.14159265358979 / 180.0;
    halfFovy *= radians_per_degree;
    // Get width and height for ease of use in matrices
    double w = src.cols;
    double h = src.rows;
    // Scale applied to image overall
    double scale = 1;
    // Find the maximum size of a rotated, projected image
    double d=hypot(w,h);
    // Compute size of dst image
    double sideLength=scale*d/cos(halfFovy);
    cv::Mat dst(cv::Size(sideLength,sideLength), CV_64F);
    // Compute focal length of virtual camera (the observer)
    double f=d/(2.0*tan(halfFovy));
    // Compute the distance to the near and far planes of focus
    double fminus=f-(d/2.0);
    double fplus=f+(d/2.0);
    // Projection 2D -> 3D matrix; corners(x,y,z,w) where w is a homogeneous scalar
    cv::Mat srcCorners = (cv::Mat_<double>(4, 4) <<
                  -w/2, w/2,  w/2, -w/2,
                   h/2, h/2, -h/2, -h/2,
                   0,    0,     0,   0,
                   1 ,   1,     1,   1);
    // Compute direction specific rotation matrices around the X, Y, and Z axis
    cv::Mat RX = (cv::Mat_<double>(4, 4) <<
              1,    0,         0,     0,
              0, cos(phi), -sin(phi), 0,
              0, sin(phi),  cos(phi), 0,
              0,    0,         0,     1);
    cv::Mat RY = (cv::Mat_<double>(4, 4) <<
               cos(psi), 0, sin(psi), 0,
                  0,     1,    0,     0,
              -sin(psi), 0, cos(psi), 0,
                  0,     0,    0,     1);
    cv::Mat RZ = (cv::Mat_<double>(4, 4) <<
              cos(theta), -sin(theta), 0, 0,
              sin(theta),  cos(theta), 0, 0,
                 0,           0,       1, 0,
                 0,           0,       0, 1);
    // Compute overall rotation matrix as linear multiplication of directional rotation matrices
    cv::Mat R = RX * RY * RZ;
    // Create translation matrix
    cv::Mat T = cv::Mat::eye(4,4,CV_64FC1);
    T.at<double>(2,3)=-f;
    //Projection matrix, 3D -> 2D; Camera intrinsics
    cv::Mat P = cv::Mat::zeros(4,4,CV_64FC1);
    P.at<double>(0,0)=P.at<double>(1,1)=1.0/tan(halfFovy);
    P.at<double>(2,2)=-(fplus+fminus)/(fplus-fminus);
    P.at<double>(2,3)=-(2.0*fplus*fminus)/(fplus-fminus);
    P.at<double>(3,2)=-1.0;
    // Compute final transformation matrix
    cv::Mat trans = P * T * R;
//    cout << trans << endl;
    // Apply matrix transformation to src corners to find corners in the dst image (map corners)
    cv::Mat mapCorners = trans*srcCorners;
    // Divide x and y values by the projection scalars (w)
    cv::divide(mapCorners.row(0), mapCorners.row(3), mapCorners.row(0));
    cv::divide(mapCorners.row(1), mapCorners.row(3), mapCorners.row(1));
    // Shift to remove neagtive coordinates and scale to dst size
    mapCorners += 1;
    mapCorners *= (sideLength/2);
    // Add the center values back in to remove negative coordinates
    srcCorners.row(0) += (w/2);
    srcCorners.row(1) += (h/2);
    // Populate point vectors with (x,y) values
    vector<cv::Point2f> ptsIn(4);
    for(int i=0;i<4;i++){
        ptsIn[i] = cv::Point2f(srcCorners.at<double>(0,i), srcCorners.at<double>(1,i));
        ptsOut[i] = cv::Point2f(mapCorners.at<double>(0,i), mapCorners.at<double>(1,i));
    }
    // Perform a quad-to-quad perspective transform
    ipcv::MapQ2Q(src, dst, ptsIn, ptsOut, map1, map2);
  return true;
}
}
