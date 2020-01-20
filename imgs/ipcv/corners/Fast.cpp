/** Implementation file for FAST corner detection
*
*  \file ipcv/Corners/Fast.cpp
*  \author Jacob Stevens (jss8649@rit.edu)
*  \date 02 11 2019
*/

#include "Corners.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"

using namespace std;

namespace ipcv {

/** Apply the FAST corner detector to a color image
 *
 *  \param[in] src     source cv::Mat of CV_8UC3
 *  \param[out] dst    destination cv:Mat of CV_8UC3
 *  \param[in] difference_threshold
 *                     brightness threshold to be used to determine whether
 *                     a surrounding pixels is brighter than or darker than
 *                     the candidate corner pixel
 *  \param[in] contiguous_threshold
 *                     number of contiguous pixels that must appear in
 *                     sequence in order for a candidate pixel to be
 *                     considered a corner pixel
 *  \param[in] nonmaximal_suppression
 *                     boolean parameter indicating whether non-maximal
 *                     suppression should be used to eliminate "clumping"
 *                     of identified corner points
 */
bool Fast(const cv::Mat& src, cv::Mat& dst, const int difference_threshold,
          const int contiguous_threshold, const bool nonmaximal_supression) {
    cv::Mat src_gray;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    dst.create(src.size(), CV_8UC1);
    // Create a list of values that represent a circle of radius three in image pixels
    int circle[25];
    circle[0] = 0 + src.step * 3;
    circle[1] = 1 + src.step * 3;
    circle[2] = 2 + src.step * 2;
    circle[3] = 3 + src.step * 1;
    circle[4] = 3 + src.step * 0;
    circle[5] = 3 + src.step * -1;
    circle[6] = 2 + src.step * -2;
    circle[7] = 1 + src.step * -3;
    circle[8] = 0 + src.step * -3;
    circle[9] = -1 + src.step * -3;
    circle[10] = -2 + src.step * -2;
    circle[11] = -3 + src.step * -1;
    circle[12] = -3 + src.step * 0;
    circle[13] = -3 + src.step * 1;
    circle[14] = -2 + src.step * 2;
    circle[15] = -1 + src.step * 3;
    for(int k = 16; k < 25; k++) circle[k] = circle[k - 16];
    
    int radius = 4;
    int diameter = 2*radius+1;
    cv::Rect r;
    int topAdd, topSub, botAdd, botSub, leftAdd, leftSub, rightAdd, rightSub;
    // Loop through every pixel of the src image within the diameter of the search circle
    for (int i = diameter;i<src.rows-diameter;i++){
        // Create pointers into the src and dst image rows
        const uchar* ptr = src_gray.ptr<uchar>(i);
        uchar* output = dst.ptr<uchar>(i);
        for (int j = diameter;j<src.cols-diameter;j++, ptr++){
            int above = 0;
            int below = 0;
            // Check if the top or bottom circle pixels are outside the diff thresh
            topAdd = ptr[circle[0]]+difference_threshold;
            if (topAdd > ptr[0]) {
                above++;
            }
            botAdd = ptr[circle[8]]+difference_threshold;
            if (botAdd > ptr[0]) {
                above++;
            }
            topSub = ptr[circle[0]]-difference_threshold;
            if (topSub < ptr[0]) {
                below++;
            }
            botSub = ptr[circle[8]]-difference_threshold;
            if (botSub < ptr[0]) {
                below++;
            }
            // Check if the left or right circle pixels are outside the diff thresh
            if (above == 0 && below == 0){
                continue;
            }
            leftAdd = ptr[circle[4]]+difference_threshold;
            if (leftAdd > ptr[0]) {
                above++;
            }
            rightAdd = ptr[circle[12]]+difference_threshold;
            if (rightAdd > ptr[0]) {
                above++;
            }
            leftSub = ptr[circle[4]]-difference_threshold;
            if (leftSub < ptr[0]) {
                below++;
            }
            rightSub = ptr[circle[12]]-difference_threshold;
            if (rightSub < ptr[0]) {
                below++;
            }
            // If more than 3 pixels are above thresh, check the rest
            if (above >= 3) {
                // Create value threshold
                int vt = ptr[0] + difference_threshold, count = 0;
                // Loop through each pixel in the circle
                for(int k = 0; k < 25; k++ )
                {
                    int x = ptr[circle[k]];
                    if(x > vt)
                    {
                        // If the number of connected pixels is greater than the thresh, pixel is a corner
                        if( ++count > contiguous_threshold)
                        {
                            output[j] = 255;
                        }
                    }
                    // If not a connected pixel, restart the count
                    else
                        count = 0;
                }
            }
            // If more than 3 pixels are below thresh, check the rest
            else if (below >= 3) {
                int vt = ptr[0] - difference_threshold, count = 0;
                // Loop through each pixel in the circle
                for(int k = 0; k < 25; k++ )
                {
                    int x = ptr[circle[k]];
                    if(x < vt)
                    {
                        // If the number of connected pixels is greater than the thresh, pixel is a corner
                        if( ++count > contiguous_threshold)
                        {
                            output[j] = 255;
                        }
                    }
                    // If not a connected pixel, restart the count
                    else
                        count = 0;
                }
            }
        }
    }
  return true;
}
}
