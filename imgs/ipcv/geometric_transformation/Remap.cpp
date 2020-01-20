/** Implementation file for remapping source values to map locations
 *
 *  \file ipcv/geometric_transformation/Remap.cpp
 *  \author Jacob Stevens (jss8649@rit.edu)
 *  \date 05 09 2019
 */

#include "Remap.h"

#include <iostream>

using namespace std;

namespace ipcv {

/** Remap source values to the destination array at map1, map2 locations
 *
 *  \param[in] src            source cv::Mat of CV_8UC3
 *  \param[out] dst           destination cv::Mat of CV_8UC3 for remapped values
 *  \param[in] map1           cv::Mat of CV_32FC1 (size of the destination map)
 *                            containing the horizontal (x) coordinates at
 *                            which to resample the source data
 *  \param[in] map2           cv::Mat of CV_32FC1 (size of the destination map)
 *                            containing the vertical (y) coordinates at
 *                            which to resample the source data
 *  \param[in] interpolation  interpolation to be used for resampling
 *  \param[in] border_mode    border mode to be used for out of bounds pixels
 *  \param[in] border_value   border value to be used when constant border mode
 *                            is to be used
 */

bool Remap(const cv::Mat& src, cv::Mat& dst, const cv::Mat& map1,
           const cv::Mat& map2, const Interpolation interpolation,
           const BorderMode border_mode, const uint8_t border_value) {
    dst = cv::Mat::zeros(map1.size(), src.type());

    const uchar* srcptr = src.data;
    uchar* dstptr = dst.data;
    int x, y;
    // Index into the destination image
    for (int i = 0; i<dst.rows; i++){
        // Create pointers to the map(s) rows
        const float* map1row = map1.ptr<float>(i);
        const float* map2row = map2.ptr<float>(i);
        for (int j = 0; j<dst.cols; j++){
            // Cast the map values as integers
            x = map1row[j], y = map2row[j];
            // Only map the values that are inside the src's coordinates
            if (x > 0 && y > 0 && x < src.cols && y < src.rows){
                // Switch between interpolation types
                switch (interpolation) {
                    case ipcv::Interpolation::NEAREST:{
                        dstptr[i*dst.step[0] + j*dst.channels()] = srcptr[y*src.step[0] + x*src.channels()];
                        // Handle the color image case
                        if (src.channels() == 3){
                            dstptr[i*dst.step[0] + j*dst.channels() + 1] = srcptr[y*src.step[0] + x*src.channels() + 1];
                            dstptr[i*dst.step[0] + j*dst.channels() + 2] = srcptr[y*src.step[0] + x*src.channels() + 2];
                        }
                        break;
                    }
                    case ipcv::Interpolation::LINEAR:{
                        int topL, topR, botL, botR;
                        // Find the remainder decimal values of the coordinates
                        float xDec = map1row[j]-x, yDec = map2row[j]-y;
                        float dst_val_0, dst_val_1;
                        // Find the pixels inside the local 2x2 neighborhood
                        topL = srcptr[y*src.step[0] + x*src.step[1]];
                        topR = srcptr[y*src.step[0] + (x+1)*src.step[1]];
                        botL = srcptr[(y+1)*src.step[0] + x*src.step[1]];
                        botR = srcptr[(y+1)*src.step[0] + (x+1)*src.step[1]];
                        // Calculate the bilinear interpolation for the first(or blue) channel
                        dst_val_0 = (topR - topL) * xDec + topL;
                        dst_val_1 = (botR - botL) * xDec + botL;
                        dstptr[i*dst.step[0] + j*dst.channels()] = floor((dst_val_1 - dst_val_0) * yDec + dst_val_0);
                        // Handle the color image case
                        if (src.channels() == 3) {
                            // Find the pixels inside the local 2x2 neighborhood for green channel
                            topL = srcptr[y*src.step[0] + x*src.step[1] + 1];
                            topR = srcptr[y*src.step[0] + (x+1)*src.step[1] + 1];
                            botL = srcptr[(y+1)*src.step[0] + x*src.step[1] + 1];
                            botR = srcptr[(y+1)*src.step[0] + (x+1)*src.step[1] + 1];
                            // Calculate the bilinear interpolation for the green channel
                            dst_val_0 = (topR - topL) * xDec + topL;
                            dst_val_1 = (botR - botL) * xDec + botL;
                            dstptr[i*dst.step[0] + j*dst.channels() + 1] = floor((dst_val_1 - dst_val_0) * yDec + dst_val_0);
                            // Find the pixels inside the local 2x2 neighborhood for red channel
                            topL = srcptr[y*src.step[0] + x*src.step[1] + 2];
                            topR = srcptr[y*src.step[0] + (x+1)*src.step[1] + 2];
                            botL = srcptr[(y+1)*src.step[0] + x*src.step[1] + 2];
                            botR = srcptr[(y+1)*src.step[0] + (x+1)*src.step[1] + 2];
                            // Calculate the bilinear interpolation for the red channel
                            dst_val_0 = (topR - topL) * xDec + topL;
                            dst_val_1 = (botR - botL) * xDec + botL;
                            dstptr[i*dst.step[0] + j*dst.channels() + 2] = floor((dst_val_1 - dst_val_0) * yDec + dst_val_0);
                        }
                        break;
                    }
                    // Default is Nearest Neighbor Interpolation
                    default:{
                        dstptr[i*dst.step[0] + j*dst.channels()] = srcptr[y*src.step[0] + x*src.channels()];
                        if (src.channels() == 3){
                            dstptr[i*dst.step[0] + j*dst.channels() + 1] = srcptr[y*src.step[0] + x*src.channels() + 1];
                            dstptr[i*dst.step[0] + j*dst.channels() + 2] = srcptr[y*src.step[0] + x*src.channels() + 2];
                        }
                        break;
                    }
                }
            }
            // Values outsides src's coordinates are bordered
            else {
                // Switch between interpolation types
                switch (border_mode) {
                    case ipcv::BorderMode::REPLICATE:{
                        // clip the map values between 0 and the sources image's boundary
                        x = clamp(x, 0, src.cols-1);
                        y = clamp(y, 0, src.rows-1);
                        dstptr[i*dst.step[0] + j*dst.channels()] = srcptr[y*src.step[0] + x*src.channels()];
                        // Handle the color image case
                        if (src.channels() == 3){
                            dstptr[i*dst.step[0] + j*dst.channels() + 1] = srcptr[y*src.step[0] + x*src.channels() + 1];
                            dstptr[i*dst.step[0] + j*dst.channels() + 2] = srcptr[y*src.step[0] + x*src.channels() + 2];
                        }
                        break;
                    }
                    case ipcv::BorderMode::CONSTANT:{
                        dstptr[i*dst.step[0] + j*dst.channels()] = border_value;
                        // Handle the color image case
                        if (src.channels() == 3){
                            dstptr[i*dst.step[0] + j*dst.channels() + 1] = border_value;
                            dstptr[i*dst.step[0] + j*dst.channels() + 2] = border_value;
                        }
                        break;
                    }
                    // If no border value is given, leave the value at 0
                    default:{
                        break;
                    }
                }
            }
        }
    }
    return true;
}
}
