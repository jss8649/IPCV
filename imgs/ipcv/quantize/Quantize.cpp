/** Implementation file for image quantization
 *
 *  \file ipcv/quantize/quantize.cpp
 *  \author Jacob Stevens (jss8649@rit.edu)
 *  \date 02 09 2019
 */

#include "Quantize.h"
#include <bitset>
#include <iostream>

using namespace std;

/** Perform uniform grey-level quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 */

void Uniform(const cv::Mat& src, const int quantization_levels, cv::Mat& dst) {

    dst.create(src.size(), src.type());
    // Find the Maximum pixel value for the source's given bit depth
    int maxVal = 255;
    // Calculate the "width" of the quantization levels
    int binSize = maxVal/quantization_levels;
    
    for(int i = 0; i < src.rows; i++){
        for(int j = 0; j < src.cols; j++){
            // Grab the pixel(s) value at every location in the image
            auto value = src.at<cv::Vec3b>(i,j);
            // Perform the quantization by binning the pixels via integer division
            value[0] = value[0]/binSize;
            value[1] = value[1]/binSize;
            value[2] = value[2]/binSize;
            // Assign the quantized values to the destination image (to be scaled back later)
            dst.at<cv::Vec3b>(i,j)[0] = value[0];
            dst.at<cv::Vec3b>(i,j)[1] = value[1];
            dst.at<cv::Vec3b>(i,j)[2] = value[2];
        }
    }
}

/** Perform improved grey scale quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 */
void Igs(const cv::Mat& src, const int quantization_levels, cv::Mat& dst) {
    
    dst.create(src.size(), CV_8UC3);
    // Find the number of quantized bit levels by calculating the log base 2 floor
    // ex: log2(9) = 3
    const int bitLevels = log2(quantization_levels);
    
    vector<string> upperBits(3);
    vector<string> lowerBits(3);
    vector<string> bits(3);
    // Instantiate the first upper and lower bit values to be 0
    vector<long> upperBitsVal(3, 0);
    vector<long> lowerBitsVal(3, 0);
    vector<long> newVal(3);
    for(int i = 0; i < src.rows; i++){
        for(int j = 0; j < src.cols; j++){
            // Grab the pixel(s) value at every location in the image
            auto value = src.at<cv::Vec3b>(i,j);
            // Add the lower bit value of the previous pixel
            newVal[0] = value[0]+lowerBitsVal[0];
            newVal[1] = value[1]+lowerBitsVal[1];
            newVal[2] = value[2]+lowerBitsVal[2];
            // Convert the current pixel value to binary
            bits[0] = bitset<8>(newVal[0]).to_string();
            bits[1] = bitset<8>(newVal[1]).to_string();
            bits[2] = bitset<8>(newVal[2]).to_string();
            // Segment the new current bit into it's upper and lower portions
            upperBits[0] = bits[0].substr(0,8-bitLevels);
            upperBits[1] = bits[1].substr(0,8-bitLevels);
            upperBits[2] = bits[2].substr(0,8-bitLevels);
            lowerBits[0] = bits[0].substr(8-bitLevels,string::npos);
            lowerBits[1] = bits[1].substr(8-bitLevels,string::npos);
            lowerBits[2] = bits[2].substr(8-bitLevels,string::npos);
            // Perform the quantization by assigning the upper bits to the destination image and sending the lower bits to the next pixel
            if(bitLevels == 7){
                dst.at<cv::Vec3b>(i,j)[0] = bitset<7>(upperBits[0]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[1] = bitset<7>(upperBits[1]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[2] = bitset<7>(upperBits[2]).to_ulong();
                lowerBitsVal[0] = bitset<1>(lowerBits[0]).to_ulong();
                lowerBitsVal[1] = bitset<1>(lowerBits[1]).to_ulong();
                lowerBitsVal[2] = bitset<1>(lowerBits[2]).to_ulong();
            }
            else if (bitLevels == 6){
                dst.at<cv::Vec3b>(i,j)[0] = bitset<6>(upperBits[0]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[1] = bitset<6>(upperBits[1]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[2] = bitset<6>(upperBits[2]).to_ulong();
                lowerBitsVal[0] = bitset<2>(lowerBits[0]).to_ulong();
                lowerBitsVal[1] = bitset<2>(lowerBits[1]).to_ulong();
                lowerBitsVal[2] = bitset<2>(lowerBits[2]).to_ulong();
            }
            else if (bitLevels == 5){
                dst.at<cv::Vec3b>(i,j)[0] = bitset<5>(upperBits[0]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[1] = bitset<5>(upperBits[1]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[2] = bitset<5>(upperBits[2]).to_ulong();
                lowerBitsVal[0] = bitset<3>(lowerBits[0]).to_ulong();
                lowerBitsVal[1] = bitset<3>(lowerBits[1]).to_ulong();
                lowerBitsVal[2] = bitset<3>(lowerBits[2]).to_ulong();
            }
            else if (bitLevels == 4){
                dst.at<cv::Vec3b>(i,j)[0] = bitset<4>(upperBits[0]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[1] = bitset<4>(upperBits[1]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[2] = bitset<4>(upperBits[2]).to_ulong();
                lowerBitsVal[0] = bitset<4>(lowerBits[0]).to_ulong();
                lowerBitsVal[1] = bitset<4>(lowerBits[1]).to_ulong();
                lowerBitsVal[2] = bitset<4>(lowerBits[2]).to_ulong();
            }
            else if (bitLevels == 3){
                dst.at<cv::Vec3b>(i,j)[0] = bitset<3>(upperBits[0]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[1] = bitset<3>(upperBits[1]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[2] = bitset<3>(upperBits[2]).to_ulong();
                lowerBitsVal[0] = bitset<5>(lowerBits[0]).to_ulong();
                lowerBitsVal[1] = bitset<5>(lowerBits[1]).to_ulong();
                lowerBitsVal[2] = bitset<5>(lowerBits[2]).to_ulong();
            }
            else if (bitLevels == 2){
                dst.at<cv::Vec3b>(i,j)[0] = bitset<2>(upperBits[0]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[1] = bitset<2>(upperBits[1]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[2] = bitset<2>(upperBits[2]).to_ulong();
                lowerBitsVal[0] = bitset<2>(lowerBits[0]).to_ulong();
                lowerBitsVal[1] = bitset<2>(lowerBits[1]).to_ulong();
                lowerBitsVal[2] = bitset<2>(lowerBits[2]).to_ulong();
            }
            else if (bitLevels == 1){
                dst.at<cv::Vec3b>(i,j)[0] = bitset<1>(upperBits[0]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[1] = bitset<1>(upperBits[1]).to_ulong();
                dst.at<cv::Vec3b>(i,j)[2] = bitset<1>(upperBits[2]).to_ulong();
                lowerBitsVal[0] = bitset<7>(lowerBits[0]).to_ulong();
                lowerBitsVal[1] = bitset<7>(lowerBits[1]).to_ulong();
                lowerBitsVal[2] = bitset<7>(lowerBits[2]).to_ulong();
            }
        }
    }
}

namespace ipcv {

bool Quantize(const cv::Mat& src, const int quantization_levels,
              const QuantizationType quantization_type, cv::Mat& dst) {
  dst.create(src.size(), src.type());

  switch (quantization_type) {
    case QuantizationType::uniform:
      Uniform(src, quantization_levels, dst);
      break;
    case QuantizationType::igs:
      Igs(src, quantization_levels, dst);
      break;
    default:
      cerr << "Specified quantization type is unsupported" << endl;
      return false;
  }

  return true;
}
}
