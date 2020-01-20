/** Implementation file for CFA demosaic function using homogeneity-directed interpolation
 *
 *  \file ipcv/demosaic/Homogeneity.cpp
 *  \author Jacob Stevens (jss8649@rit.edu)
 *  \date 20 Jan 2018
 */

#include <iostream>

#include <opencv2/imgproc.hpp>

#include <opencv2/highgui.hpp>

#include "imgs/ipcv/utils/Utils.h"

#include "Bilinear.h"

using namespace std;

namespace ipcv {
    
double medianMat(cv::Mat Input){
    Input = Input.reshape(0,1); // spread Input Mat to single row
    std::vector<double> vecFromMat;
    Input.copyTo(vecFromMat); // Copy Input Mat to vector vecFromMat
    std::sort( vecFromMat.begin(), vecFromMat.end() ); // sort vecFromMat
    if (vecFromMat.size()%2==0) {return (vecFromMat[vecFromMat.size()/2-1]+vecFromMat[vecFromMat.size()/2])/2;} // in case of even-numbered matrix
    return vecFromMat[(vecFromMat.size()-1)/2]; // odd-number of elements in matrix
}

cv::Mat Homogeneity(const cv::Mat& src, string pattern) {
    // Create blue (B) and red (R) filter location masks
    int b_row;
    int b_col;
    int r_row;
    int r_col;
    if (pattern == "GBRG") {
        b_row = 0;
        b_col = 1;
        r_row = 1;
        r_col = 0;
        } else if (pattern == "GRBG") {
        b_row = 1;
        b_col = 0;
        r_row = 0;
        r_col = 1;
        } else if (pattern == "BGGR") {
        b_row = 0;
        b_col = 0;
        r_row = 1;
        r_col = 1;
        } else if (pattern == "RGGB") {
        b_row = 1;
        b_col = 1;
        r_row = 0;
        r_col = 0;
    }
    else {
        cerr << "Invalid CFA pattern provided: " << pattern << endl;
        exit(EXIT_FAILURE);
    }
    cv::Mat b_mask = cv::Mat::zeros(src.size(), CV_16UC1);
    for (int r = b_row; r < src.rows; r += 2) {
        for (int c = b_col; c < src.cols; c += 2) {
          b_mask.at<uint16_t>(r, c) = 1;
        }
    }
  cv::Mat r_mask = cv::Mat::zeros(src.size(), CV_16UC1);
  for (int r = r_row; r < src.rows; r += 2) {
    for (int c = r_col; c < src.cols; c += 2) {
      r_mask.at<uint16_t>(r, c) = 1;
    }
  }

  // Create green (G) filter location mask
  cv::Mat g_mask = cv::Mat::ones(src.size(), CV_16UC1);
  cv::subtract(g_mask, b_mask, g_mask);
  cv::subtract(g_mask, r_mask, g_mask);

  // Initialize interpolated B, G, and R channels
  cv::Mat B;
  cv::multiply(src, b_mask, B, 1, CV_32FC1);
  cv::Mat G;
  cv::multiply(src, g_mask, G, 1, CV_32FC1);
  cv::Mat R;
  cv::multiply(src, r_mask, R, 1, CV_32FC1);
    
    // Split the green channel into even and odd columns
    cv::Mat G0Hor = cv::Mat::zeros(src.size(), CV_32FC1);
    cv::Mat G1Hor = cv::Mat::zeros(src.size(), CV_32FC1);
    for (int c = 0; c < src.cols; c += 2) {
        G.col(c).copyTo(G0Hor.col(c));
        G.col(c+1).copyTo(G1Hor.col(c+1));
    }

    // Create the filter
    cv::Mat h1(1, 5, CV_32FC1);
    h1.at<float>(0,0) = 0.;
    h1.at<float>(0,1) = 0.5;
    h1.at<float>(0,2) = 0.;
    h1.at<float>(0,3) = 0.5;
    h1.at<float>(0,4) = 0.;
    cv::Mat h0(1, 5, CV_32FC1);
    h0.at<float>(0,0) = -0.25;
    h0.at<float>(0,1) = 0.;
    h0.at<float>(0,2) = 0.5;
    h0.at<float>(0,3) = 0.;
    h0.at<float>(0,4) = -0.25;
    
    // Run the filter over the color channels in the horizonal direction
    cv::Mat filterH1G0, filterH0R, filterH1G1, filterH0B;
    cv::filter2D(G0Hor, filterH1G0, CV_32FC1, h1);
    cv::filter2D(R, filterH0R, CV_32FC1, h0);
    cv::filter2D(G1Hor, filterH1G1, CV_32FC1, h1);
    cv::filter2D(B, filterH0B, CV_32FC1, h0);
    
    
    // Create the green interpolated image in the horizonal direction
    cv::Mat Rhor, Bhor, temp, GhorRed, GhorBlue, Gs(src.size(), CV_32FC1);
    GhorRed = G0Hor + filterH1G0 + filterH0R;
    GhorBlue = G1Hor + filterH1G1 + filterH0B;
    cv::Mat Ghor = GhorBlue + GhorRed;
    
    // Create the red interpolated image in the horizonal direction
    cv::multiply(Ghor, r_mask, Gs, 1, CV_32FC1);
    Rhor = R-Gs;
    // Set bounds for the iterpolation domain
    int ul_col = 2;
    int lr_row = src.rows - 2;
    int lr_col = src.cols - 2;
    for (int r = r_row; r < lr_row; r += 2) {
        for (int c = ul_col; c < lr_col; c++) {
            if (r_mask.at<uint16_t>(r, c) == 0) {
                Rhor.at<float>(r, c) =
                (Rhor.at<float>(r, c - 1) + Rhor.at<float>(r, c + 1)) / 2;
            }
        }
    }
    for (int r = r_row + 1; r < lr_row; r += 2) {
        for (int c = ul_col; c < lr_col; c++) {
            if (r_mask.at<uint16_t>(r, c) == 0) {
                Rhor.at<float>(r, c) =
                (Rhor.at<float>(r - 1, c) + Rhor.at<float>(r + 1, c)) / 2;
            }
        }
    }
    Rhor += Ghor;
    
    // Create the blue interpolated image in the horizonal direction
    Gs = filterH1G1 + filterH0B;
    Bhor = B-Gs;
    for (int r = b_row; r < lr_row; r += 2) {
        for (int c = ul_col; c < lr_col; c++) {
            if (b_mask.at<uint16_t>(r, c) == 0) {
                Bhor.at<float>(r, c) =
                (Bhor.at<float>(r, c - 1) + Bhor.at<float>(r, c + 1)) / 2;
            }
        }
    }
    
    for (int r = b_row + 1; r < lr_row; r += 2) {
        for (int c = ul_col; c < lr_col; c++) {
            if (b_mask.at<uint16_t>(r, c) == 0) {
                Bhor.at<float>(r, c) =
                (Bhor.at<float>(r - 1, c) + Bhor.at<float>(r + 1, c)) / 2;
            }
        }
    }
    Bhor += Ghor;
    
    
    // Split the green channel into even and odd rows
    cv::Mat G0Vert = cv::Mat::zeros(src.size(), CV_32FC1);
    cv::Mat G1Vert = cv::Mat::zeros(src.size(), CV_32FC1);
    for (int r = 0; r < src.rows; r += 2) {
        G.row(r).copyTo(G0Vert.row(r));
        G.row(r+1).copyTo(G1Vert.row(r+1));
    }
    // Run the filter over the color channels in the vertical direction
    cv::filter2D(G0Vert, filterH1G0, CV_32FC1, h1.t());
    cv::filter2D(R, filterH0R, CV_32FC1, h0.t());
    cv::filter2D(G1Vert, filterH1G1, CV_32FC1, h1.t());
    cv::filter2D(B, filterH0B, CV_32FC1, h0.t());
    cv::Mat GvertRed = G0Vert + filterH1G1 + filterH0R;
    cv::Mat GvertBlue = G1Vert + filterH1G0 + filterH0B;
    cv::Mat Gvert = GvertRed + GvertBlue;
    
    // Create the red interpolated image in the vertical direction
    cv::Mat Rvert, Bvert;
    Gs = filterH1G1 + filterH0R;
    Rvert = R-Gs;
    for (int r = r_row; r < lr_row; r += 2) {
        for (int c = ul_col; c < lr_col; c++) {
            if (r_mask.at<uint16_t>(r, c) == 0) {
                Rvert.at<float>(r, c) =
                (Rvert.at<float>(r, c - 1) + Rvert.at<float>(r, c + 1)) / 2;
            }
        }
    }
    for (int r = r_row + 1; r < lr_row; r += 2) {
        for (int c = ul_col; c < lr_col; c++) {
            if (r_mask.at<uint16_t>(r, c) == 0) {
                Rvert.at<float>(r, c) =
                (Rvert.at<float>(r - 1, c) + Rvert.at<float>(r + 1, c)) / 2;
            }
        }
    }
    Rvert += Gvert;
    
    // Create the blue interpolated image in the vertical direction
    Gs = filterH1G0 + filterH0B;
    Bvert = B-Gs;
    for (int r = b_row; r < lr_row; r += 2) {
        for (int c = ul_col; c < lr_col; c++) {
            if (b_mask.at<uint16_t>(r, c) == 0) {
                Bvert.at<float>(r, c) =
                (Bvert.at<float>(r, c - 1) + Bvert.at<float>(r, c + 1)) / 2;
            }
        }
    }
    
    for (int r = b_row + 1; r < lr_row; r += 2) {
        for (int c = ul_col; c < lr_col; c++) {
            if (b_mask.at<uint16_t>(r, c) == 0) {
                Bvert.at<float>(r, c) =
                (Bvert.at<float>(r - 1, c) + Bvert.at<float>(r + 1, c)) / 2;
            }
        }
    }
    Bvert += Gvert;
    
    cv::Mat tempHor, tempVert, Hor, Vert;
    cv::Mat channels[3];
    channels[0] = Bhor;
    channels[1] = Ghor;
    channels[2] = Rhor;
    cv::merge(channels, 3, Hor);
    channels[0] = Bvert;
    channels[1] = Gvert;
    channels[2] = Rvert;
    cv::merge(channels, 3, Vert);
    

    int filterDiameter = 3;
    int filterRadius = (filterDiameter-1)/2;
    cv::copyMakeBorder(Hor, tempHor, filterRadius, filterRadius, filterRadius, filterRadius, cv::BORDER_CONSTANT);
    cv::copyMakeBorder(Vert, tempVert, filterRadius, filterRadius, filterRadius, filterRadius, cv::BORDER_CONSTANT);

    cv::cvtColor(tempHor, tempHor, cv::COLOR_BGR2Lab);
    cv::cvtColor(tempVert, tempVert, cv::COLOR_BGR2Lab);
    // Create a copy of the src image in the Lab colorspace
    cv::Rect r;
    tempHor.convertTo(tempHor, CV_32FC3);
    tempVert.convertTo(tempVert, CV_32FC3);
    // Split the Lab image into 3 seperate images
    cv::Mat LabHor[3];
    cv::split(tempHor, LabHor);
    cv::Mat LabVert[3];
    cv::split(tempVert, LabVert);
    cv::Mat cutLHor, cutAHor, cutBHor, cutLVert, cutAVert, cutBVert;
    cv::Mat rangeLHor, rangeLVert;
    
    float thresh2 = 0.5, thresh3 = 0.5;
    cv::Mat homoHor(src.size(), CV_32FC1);
    cv::Mat homoVert(src.size(), CV_32FC1);

    // Create the homogeneity map
    for (int i = 0;i<src.rows;i++){
        for (int j = 0;j<src.cols;j++){
            // Create a small window of the Lab image
            r = cv::Rect(j, i, filterDiameter, filterDiameter);
            cutLHor = LabHor[0](r);
            cutAHor = LabHor[1](r);
            cutBHor = LabHor[2](r);
            
            cutLVert = LabVert[0](r);
            cutAVert = LabVert[1](r);
            cutBVert = LabVert[2](r);
            
            float LHor = tempHor.at<cv::Vec3f>(i+filterRadius+1,j+filterRadius+1)[0];
            float aHor = tempHor.at<cv::Vec3f>(i+filterRadius+1,j+filterRadius+1)[1];
            float bHor = tempHor.at<cv::Vec3f>(i+filterRadius+1,j+filterRadius+1)[2];

            float LVert = tempVert.at<cv::Vec3f>(i+filterRadius+1,j+filterRadius+1)[0];
            float aVert = tempVert.at<cv::Vec3f>(i+filterRadius+1,j+filterRadius+1)[1];
            float bVert = tempVert.at<cv::Vec3f>(i+filterRadius+1,j+filterRadius+1)[2];
            // Calculate the difference from the window image's pixels to
            // the original image's pixel value
            rangeLHor = abs(cutLHor - LHor);
            cv::threshold(rangeLHor, rangeLHor, thresh2, 1, cv::THRESH_TOZERO_INV);
            
            rangeLVert = abs(cutLVert - LVert);
            cv::threshold(rangeLVert, rangeLVert, thresh2, 1, cv::THRESH_TOZERO_INV);

            cv::Mat distabHor, distabVert;
            
            cv::Mat diffaHor, diffbHor;
            cv::pow(cutAHor-aHor, 2, diffaHor);
            cv::pow(cutBHor-bHor, 2, diffbHor);
            cv::sqrt(diffaHor+diffbHor, distabHor);
            cv::threshold(distabHor, distabHor, thresh3, 1, cv::THRESH_TOZERO_INV);
            cv::Mat UHor = distabHor * rangeLHor;
            
            cv::Mat diffaVert, diffbVert;
            cv::pow(cutAVert-aVert, 2, diffaVert);
            cv::pow(cutBVert-bVert, 2, diffbVert);
            cv::sqrt(diffaVert+diffbVert, distabVert);
            cv::threshold(distabVert, distabVert, thresh3, 1, cv::THRESH_TOZERO_INV);
            cv::Mat UVert = distabVert * rangeLVert;
            
            float countHor = 0;
            float countVert = 0;
            for (int r = 0; r < UHor.rows; r++) {
                for (int c = 0; c < UHor.cols; c++) {
                    if (UHor.at<float>(r,c) != 0) {
                        countHor += 1;
                    }
                    if (UVert.at<float>(r,c) != 0) {
                        countVert += 1;
                    }
                }
            }
            countHor /= (filterDiameter*filterDiameter);
            countVert /= (filterDiameter*filterDiameter);
            
            homoHor.at<float>(i,j) = countHor;
            homoVert.at<float>(i,j) = countVert;

        }
    }
    
    cv::blur(homoHor, homoHor, cv::Size_<int>(3,3));
    cv::blur(homoVert, homoVert, cv::Size_<int>(3,3));
    
    cv::Mat dst(src.size(), CV_32FC3);
    for (int i = 0;i<src.rows;i++){
        for (int j = 0;j<src.cols;j++){
            if (homoVert.at<float>(i,j) > homoHor.at<float>(i,j)) {
                dst.at<cv::Vec3f>(i,j) = Hor.at<cv::Vec3f>(i,j);
            }
            else {
                dst.at<cv::Vec3f>(i,j) = Vert.at<cv::Vec3f>(i,j);
            }
        }
    }

  return dst;
}
}
