/** Implementation file for computing the PDF from a histogram
 *
 *  \file ipcv/utils/HistogramToPdf.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 17 Mar 2018
 */

#include "HistogramToPdf.h"

namespace ipcv {

void HistogramToPdf(const cv::Mat& h, cv::Mat& pdf) {
  pdf.create(h.size(), CV_64F);
  double number_pixels = cv::sum(h.row(0))[0];
  for (int dc = 0; dc < 256; dc++) {
    pdf.at<double>(0, dc) = h.at<int>(0, dc) / number_pixels;
    pdf.at<double>(1, dc) = h.at<int>(1, dc) / number_pixels;
    pdf.at<double>(2, dc) = h.at<int>(2, dc) / number_pixels;
  }
}
}
