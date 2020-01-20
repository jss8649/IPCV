/** Implementation file for mapping a source quad on to a target quad
*
*  \file ipcv/geometric_transformation/MapQ2Q.cpp
*  \author Jacob Stevens (jss8649@rit.edu)
*  \date 02 09 2019
*/

#include "Filter2D.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

namespace ipcv {

/** Correlates an image with the provided kernel
 *
 *  \param[in] src          source cv::Mat of CV_8UC3
 *  \param[out] dst         destination cv::Mat of ddepth type
 *  \param[in] ddepth       desired depth of the destination image
 *  \param[in] kernel       convolution kernel (or rather a correlation
 *                          kernel), a single-channel floating point matrix
 *  \param[in] anchor       anchor of the kernel that indicates the relative
 *                          position of a filtered point within the kernel;
 *                          the anchor should lie within the kernel; default
 *                          value (-1,-1) means that the anchor is at the
 *                          kernel center
 *  \param[in] delta        optional value added to the filtered pixels
 *                          before storing them in dst
 *  \param[in] border_mode  pixel extrapolation method
 *  \param[in] border_value value to use for constant border mode
 */
bool Filter2D(const cv::Mat& src, cv::Mat& dst, const int ddepth,
              const cv::Mat& kernel, const cv::Point anchor, const int delta,
              const BorderMode border_mode, const uint8_t border_value) {
    
    // Ensure that the anchor point lies within the kernel
    int anchorX = abs(anchor.x);
    int anchorY = abs(anchor.y);
    if (anchorX > kernel.cols-1) anchorX = kernel.cols-1;
    if (anchorY > kernel.rows-1) anchorY = kernel.rows-1;

    // Find the amount to pad the src image on all of its sides
    int top = anchorY; int bottom = kernel.rows-1-top;
    int right = anchorX; int left = kernel.cols-1-right;

    // Create a padded version of the input image
    cv::Mat srcPad; int borderType;
    switch (border_mode) {
        case ipcv::BorderMode::REPLICATE:
            borderType = 1;
            cv::copyMakeBorder(src, srcPad, top, bottom, left, right, borderType);
            break;
        case BorderMode::CONSTANT:
            borderType = 0;
            cv::copyMakeBorder(src, srcPad, top, bottom, left, right, borderType, border_value);
            break;
        // If no border mode specified use a constant border
        default:
            borderType = 0;
            cv::copyMakeBorder(src, srcPad, top, bottom, left, right, borderType, border_value);
        break;
    }
    // Check if the inout image  is color or greyscale. If it's color all three
    //channels must be processed separately
    if (src.channels() == 3){
        dst = cv::Mat(src.size(), CV_32FC3);
        // Loop through the pixels of each channel starting from where the
        // non-padded portion of the image begins
        for (int i = 0;i<src.rows;i++){
            for (int j = 0;j<src.cols;j++){
                float kerneltotalB, kerneltotalG, kerneltotalR;
                kerneltotalB = kerneltotalG = kerneltotalR = 0;
                //Loop through the kernel for each pixel
                for (int k = 0; k < kernel.rows;k++){
                    for (int l = 0; l < kernel.cols;l++){
                        // Grab the current source pixel value
                        cv::Vec3b val = srcPad.at<cv::Vec3b>(i+k,j+l);
                        float B = val[0];
                        float G = val[1];
                        float R = val[2];
                        // Calculate the cumulative sum of the kernel-pixel products
                        kerneltotalB += (B * kernel.at<float>(k,l));
                        kerneltotalG += (G * kernel.at<float>(k,l));
                        kerneltotalR += (R * kernel.at<float>(k,l));
                    }
                }
                // Write the processed pixel value to dst
                dst.at<cv::Vec3f>(i,j)[0] = cv::saturate_cast<float>(kerneltotalB + delta);
                dst.at<cv::Vec3f>(i,j)[1] = cv::saturate_cast<float>(kerneltotalG + delta);
                dst.at<cv::Vec3f>(i,j)[2] = cv::saturate_cast<float>(kerneltotalR + delta);
            }
        }
    }
    // Handle the grayscale image case
    else if (src.channels() == 1){
        dst = cv::Mat(src.size(), CV_32FC1);
        // Loop through the pixels of each channel starting from where the
        // non-padded portion of the image begins
        for (int i = 0;i<src.rows;i++){
            for (int j = 0;j<src.cols;j++){
                float kernelTotal = 0;
                //Loop through the kernel for each pixel
                for (int k = 0; k < kernel.rows;k++){
                    for (int l = 0; l < kernel.cols;l++){
                        // Grab the current source pixel value
                        float val = srcPad.at<uchar>(i+k,j+l);
                        // Calculate the cumulative sum of the kernel-pixel products
                        kernelTotal += (val*kernel.at<float>(k,l));
                    }
                }
                // Write the processed pixel value to dst
                dst.at<float>(i,j) = cv::saturate_cast<float>(kernelTotal + delta);
            }
        }
    }
    // Error handling
    else{
    cout << "This image type isn't supported." << endl;
    return false;
    }
    // Convert the dst image to the specified image type
    dst.convertTo(dst, ddepth);

    return true;
}
}
