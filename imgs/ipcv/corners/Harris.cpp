/** Implementation file for Harris corner detection
*
*  \file ipcv/corners/Harris.cpp
*  \author Jacob Stevens (jss8649@rit.edu)
*  \date 02 11 2019
*/

#include "Corners.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"

using namespace std;

namespace ipcv {

/** Apply the Harris corner detector to a color image
 *
 *  \param[in] src     source cv::Mat of CV_8UC3
 *  \param[out] dst    destination cv:Mat of CV_32FC1
 *  \param[in] sigma   standard deviation of the Gaussian blur kernel
 *  \param[in] k       free parameter in the equation
 *                        dst = (lambda1)(lambda2) - k(lambda1 + lambda2)^2
 */
bool Harris(const cv::Mat& src, cv::Mat& dst, const float sigma,
            const float k) {
    cv::Mat src_gray;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    dst.create(src_gray.size(), CV_32FC1);
    // Create the gradient images: A, B, and C
    cv::Mat Dx, Dy, A, B, C, C2, det, tr2;
    cv::Sobel(src_gray, Dx, dst.depth(), 1, 0);
    cv::Sobel(src_gray, Dy, dst.depth(), 0, 1);
    cv::pow(Dx, 2, A);
    cv::pow(Dy, 2, B);
    cv::multiply(Dx, Dy, C);
    // Apply a blur filter to the gradients to spread out the corner values for easier detections
    cv::Size_<int> radius(0,0);
    cv::GaussianBlur(A, A, radius, sigma, sigma);
    cv::GaussianBlur(B, B, radius, sigma, sigma);
    cv::GaussianBlur(C, C, radius, sigma, sigma);
    // Compute the determinant and trace-squared images
    cv::multiply(A, B, det);
    cv::pow(C, 2, C2);
    det -= C2;
    tr2 = A+B;
    cv::pow(tr2, 2, tr2);
    // Compute the corner image by simulating eigenvalue decomp
    dst = det-k*tr2;
    return true;
}
}
