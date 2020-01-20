/** Implementation file for computing the PSNR between two images
 *
 *  \file ipcv/utils/Psnr.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 28 Dec 2018
 */

#include <iostream>

#include <opencv2/imgproc.hpp>

#include "Psnr.h"

using namespace std;

namespace ipcv {

double Psnr(const cv::Mat& src1, const cv::Mat& src2, int max_value) {
  if ((src1.rows != src2.rows) || (src1.cols != src2.cols) ||
      (src1.channels() != src2.channels())) {
    cerr << "Image dimensions must match exactly for PSNR computation" << endl;
    exit(EXIT_FAILURE);
  }

//  double mse = 0;
//  for (int r = 0; r < src1.rows; r++) {
//    for (int c = 0; c < src1.cols; c++) {
//      auto value1 = src1.at<cv::Vec3b>(r, c);
//      auto value2 = src2.at<cv::Vec3b>(r, c);
//      double error0 =
//          static_cast<double>(value1[0]) - static_cast<double>(value2[0]);
//      double error1 =
//          static_cast<double>(value1[1]) - static_cast<double>(value2[1]);
//      double error2 =
//          static_cast<double>(value1[2]) - static_cast<double>(value2[2]);
//      mse += error0 * error0 + error1 * error1 + error2 * error2;
//    }
//  }
//  mse /= src1.rows * src1.cols * src1.channels();

  // Compute the squared error at each pixel/channel location
  cv::Mat se;
  cv::absdiff(src1, src2, se);
  se.convertTo(se, CV_32F);
  cv::multiply(se, se, se); 

  // Compute the sum of the squared errors across all pixels/channels
  cv::Scalar channel_sse = cv::sum(se);
  double sse = channel_sse.val[0] + channel_sse.val[1] + channel_sse.val[2];

  // Compute the mean of the sum of the squared errors
  double mse = sse / src1.rows / src1.cols / src1.channels();

  // Compute the peak signal-to-noise ratio
  double psnr = 10.0 * log10(max_value * max_value / mse);

  return psnr;
}
}
