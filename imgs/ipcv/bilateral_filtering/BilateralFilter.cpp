/** Implementation file for bilateral filtering
*
*  \file ipcv/bilateral_filtering/BilateralFilter.cpp
*  \author Jacob Stevens (jss8649@rit.edu)
*  \date 02 10 2019
*/

#include "BilateralFilter.h"
#include <iostream>

using namespace std;

namespace ipcv {

/** Bilateral filter an image
 *
 *  \param[in] src             source cv::Mat of CV_8UC3
 *  \param[out] dst            destination cv::Mat of ddepth type
 *  \param[in] sigma_distance  standard deviation of distance/closeness filter
 *  \param[in] sigma_range     standard deviation of range/similarity filter
 *  \param[in] radius          radius of the bilateral filter (if negative, use
 *                             twice the standard deviation of the distance/
 *                             closeness filter)
 *  \param[in] border_mode     pixel extrapolation method
 *  \param[in] border_value    value to use for constant border mode
 */

bool BilateralFilter(const cv::Mat& src, cv::Mat& dst,
                     const double sigma_distance, const double sigma_range,
                     const int radius, const BorderMode border_mode,
                     uint8_t border_value) {

    
    // Create the filter radius and diameter
    int filterRadius;
    int filterDiameter;
    if (radius <= 0){
        filterRadius = 2*sigma_distance;
    }
    else{
        filterRadius = radius;
    }
    filterDiameter = 2*filterRadius+1;

    // Create the closeness filter
    cv::Mat closeFilter(cv::Size(filterDiameter,filterDiameter), CV_32FC1);
    for (int r = 0; r < filterDiameter; r++) {
      for (int c = 0; c < filterDiameter; c++) {
        int y = r - filterRadius;
        int x = c - filterRadius;
        closeFilter.at<float>(r, c) = sqrt(pow(y, 2) + pow(x, 2));
      }
    }
    closeFilter /= sigma_distance;
    cv::pow(closeFilter, 2, closeFilter);
    closeFilter *= -0.5;
    cv::exp(closeFilter, closeFilter);

    int borderType;
    cv::Mat padSrc;
    // Add a image border if specified
    switch (border_mode){
    case ipcv::BorderMode::REPLICATE:
        borderType = cv::BORDER_REPLICATE;
        cv::copyMakeBorder(src, padSrc, filterRadius, filterRadius, filterRadius, filterRadius, borderType);
        break;
    case ipcv::BorderMode::CONSTANT:
        borderType = cv::BORDER_CONSTANT;
        cv::copyMakeBorder(src, padSrc, filterRadius, filterRadius, filterRadius, filterRadius, borderType, border_value);
        break;
    // If no border mode specified, use wrap border
    default:
        borderType = cv::BORDER_WRAP;
        cv::copyMakeBorder(src, padSrc, filterRadius, filterRadius, filterRadius, filterRadius, borderType);
        break;
    }
    
    cv::Mat newSrc;
    dst.create(src.size(), CV_8UC3);
    // Color image case
    if(src.channels() == 3){
        // Convert the input image from BGR to Lab
        cv::cvtColor(padSrc, padSrc, cv::COLOR_BGR2Lab);
        // Create a copy of the src image in the Lab colorspace
        cv::cvtColor(src, dst, cv::COLOR_BGR2Lab);
        padSrc.convertTo(padSrc, CV_32FC3);
        // Split the Lab image into 3 seperate images
        cv::Mat Lab[3];
        cv::split(padSrc, Lab);
        // Only apply the filter to the L channel
        newSrc = Lab[0].clone();
    }
    // Grayscale case
    else if (src.channels() == 1) {
        padSrc.convertTo(newSrc, CV_32FC1);
    }
    // Error handling
    else{
    cout << "This image type isn't supported." << endl;
    return false;
    }
    
    // Perform the filering element-wise
    cv::Mat cut, rangeFilter, bilatFilter;
    for (int i = 0;i<src.rows;i++){
        for (int j = 0;j<src.cols;j++){
            // Find the current pixel within the source image
            float point = newSrc.at<float>(i+filterRadius+1,j+filterRadius+1);
            // Cut out a window of the padded image
            cut = newSrc(cv::Rect(j, i, filterDiameter, filterDiameter)).clone();
            // Create the range filter
            rangeFilter = cut - point;
            rangeFilter /= sigma_range;
            cv::pow(rangeFilter, 2, rangeFilter);
            rangeFilter *= -0.5;
            cv::exp(rangeFilter, rangeFilter);
            // Create the bilaterial filter by multiplying the range and closeness filters elementwise
            bilatFilter = rangeFilter.mul(closeFilter);
            // Normalize the filter
            bilatFilter /= cv::norm(bilatFilter, 2);
            // Caculate the weighted sum of the filter with the window image and populate
            // the destination image
            float val = cv::sum((bilatFilter.mul(cut)))[0];
            dst.at<cv::Vec3b>(i,j)[0] = cv::saturate_cast<uchar>(val);
        }
    }
    
    // Convert back to RGB from LAB for color case
    if (src.channels() == 3) cv::cvtColor(dst, dst, cv::COLOR_Lab2BGR);
    // Reduce channel size of output image for grayscale case
    else if (src.channels() == 1){
        cv::Mat split[3];
        cv::split(dst, split);
        dst = split[0];
    }
    
    return true;
}
}

