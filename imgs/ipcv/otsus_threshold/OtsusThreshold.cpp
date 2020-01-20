/** Implementation file for finding Otsu's threshold
 *
 *  \file ipcv/otsus_threshold/OtsusThreshold.cpp
 *  \author <your name> (<your e-mail address>)
 *  \date DD MMM YYYY
 */

#include "OtsusThreshold.h"

#include <iostream>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Find Otsu's threshold for each channel of a 3-channel (color) image
 *
 *  \param[in] src          source cv::Mat of CV_8UC3
 *  \param[out] threshold   threshold values for each channel of a 3-channel
 *                          color image in cv::Vec3b
 */
bool OtsusThreshold(const cv::Mat& src, cv::Vec3b& threshold) {
    threshold = cv::Vec3b();
    // Create the histogram, pdf, and cdf, of the source image
    cv::Mat_<int> h;
    ipcv::Histogram(src, h);
    cv::Mat_<double> pdf;
    ipcv::HistogramToPdf(h, pdf);
    cv::Mat_<double> cdf;
    ipcv::HistogramToCdf(h, cdf);
    // Create a 256x3 matrix of linearly incrementing DC values
    cv::Mat linearDC;
    linearDC = cv::Mat::zeros(3, 256, CV_64FC1);
    for(int i = 0; i < 256; i++){
        linearDC.at<double>(0,i) = i;
        linearDC.at<double>(1,i) = i;
        linearDC.at<double>(2,i) = i;
    }

    // Create 256x3 matrices for the mean values and the cumulative mean values
    cv::Mat mu;
    mu.create(cdf.size(), cdf.type());
    cv::Mat muTotal;
    muTotal.create(cdf.size(), cdf.type());
    // Find the means
    cv::multiply(linearDC, pdf, mu);
    // Set the first value of the total mean values to the start of the mean values
    muTotal.at<double>(0,0) = mu.at<double>(0,0);
    muTotal.at<double>(1,0) = mu.at<double>(1,0);
    muTotal.at<double>(2,0) = mu.at<double>(2,0);
    // Create the cumulative mean values
    for(int i = 1; i < 256; i++){
        muTotal.at<double>(0,i) = muTotal.at<double>(0,i-1) + mu.at<double>(0,i);
        muTotal.at<double>(1,i) = muTotal.at<double>(1,i-1) + mu.at<double>(1,i);
        muTotal.at<double>(2,i) = muTotal.at<double>(2,i-1) + mu.at<double>(2,i);
    }
    // Grab the last values of the cumulative means and arrange them in a vector
    cv::Vec3d mean(3);
    mean = {muTotal.at<double>(0,255), muTotal.at<double>(1,255), muTotal.at<double>(2,255)};
    
    // Perform the variance calculations for each image channel
    cv::Mat numer, denom;
    cv::Mat bcVar;
    bcVar.create(cdf.size(), cdf.type());
    cv::pow(mean[0]*cdf.row(0)-muTotal.row(0), 2, numer);
    cv::pow(cdf.row(0), 2, denom);
    denom = cdf.row(0)-denom;
    cv::divide(numer, denom, bcVar.row(0));
    cv::pow(mean[1]*cdf.row(1)-muTotal.row(1), 2, numer);
    cv::pow(cdf.row(1), 2, denom);
    denom = cdf.row(1)-denom;
    cv::divide(numer, denom, bcVar.row(1));
    cv::pow(mean[2]*cdf.row(2)-muTotal.row(2), 2, numer);
    cv::pow(cdf.row(2), 2, denom);
    denom = cdf.row(2)-denom;
    cv::divide(numer, denom, bcVar.row(2));
    
    // Find the max variances for each channel and set their index(digital count) to be the threshold
    double minVal, maxVal;
    cv::Point minIdx, maxIdx;
    cv::minMaxLoc(bcVar.row(0), &minVal, &maxVal, &minIdx, &maxIdx);
    threshold[0] = maxIdx.x;
    cv::minMaxLoc(bcVar.row(1), &minVal, &maxVal, &minIdx, &maxIdx);
    threshold[1] = maxIdx.x;
    cv::minMaxLoc(bcVar.row(2), &minVal, &maxVal, &minIdx, &maxIdx);
    threshold[2] = maxIdx.x;
    
  return true;
}
}
