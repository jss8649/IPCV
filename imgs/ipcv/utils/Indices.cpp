/*
 *  \file ipcv/utils/Indices.cpp
 *  \author Jake Stevens (jss8649@rit.edu)
 *  \date 04 Aug 2019
 */

#include "Indices.h"

namespace ipcv {

cv::Mat Indices(const int rows, const int cols, const bool shift){
  cv::Mat indices;
  int index = 0;

  indices = cv::Mat::zeros(2, rows*cols, CV_32FC1);

  for (int i = 0; i<rows; i++){
    for (int j = 0; j<cols; j++){
      if (shift){
        indices.ptr<float>(0)[index] = j - cols/2;
        indices.ptr<float>(1)[index] = i - rows/2;
      }
      else {
        indices.ptr<float>(0)[index] = j;
        indices.ptr<float>(1)[index] = i;
        }
      index++;
      }
  }
  return indices;
}
}