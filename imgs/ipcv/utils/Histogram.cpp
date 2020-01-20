/** Implementation file for computing an image histogram
 *
 *  \file ipcv/utils/Histogram.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 17 Mar 2018
 */

#include "Histogram.h"

namespace ipcv {

void Histogram(const cv::Mat& src, cv::Mat& h) {
  h = cv::Mat_<int>::zeros(3, 256);
  for (int r = 0; r < src.rows; r++) {
    for (int c = 0; c < src.cols; c++) {
      auto value = src.at<cv::Vec3b>(r, c);
      h.at<int>(0, value[0])++;
      h.at<int>(1, value[1])++;
      h.at<int>(2, value[2])++;
    }
  }
}
}
