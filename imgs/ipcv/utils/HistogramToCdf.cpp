/** Implementation file for computing a CDF from a histogram
 *
 *  \file ipcv/utils/HistogramToCdf.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 17 Mar 2018
 */

#include "HistogramToCdf.h"

#include "imgs/ipcv/utils/HistogramToPdf.h"

namespace ipcv {

void HistogramToCdf(const cv::Mat& h, cv::Mat& cdf) {
  cv::Mat pdf;
  HistogramToPdf(h, pdf);

  cdf.create(h.size(), CV_64F);
  cdf.at<double>(0, 0) = pdf.at<double>(0, 0);
  cdf.at<double>(1, 0) = pdf.at<double>(1, 0);
  cdf.at<double>(2, 0) = pdf.at<double>(2, 0);
  for (int dc = 1; dc < 256; dc++) {
    cdf.at<double>(0, dc) = cdf.at<double>(0, dc - 1) + pdf.at<double>(0, dc);
    cdf.at<double>(1, dc) = cdf.at<double>(1, dc - 1) + pdf.at<double>(1, dc);
    cdf.at<double>(2, dc) = cdf.at<double>(2, dc - 1) + pdf.at<double>(2, dc);
  }
}
}
