#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

int main() {
  string filename = "../data/images/misc/lenna_color.ppm";

  cv::Mat src = cv::imread(filename, cv::IMREAD_UNCHANGED);
  cout << "Image ..." << endl;
  cout << filename << endl;
  cout << "Dimensions: " << src.rows << "x" << src.cols << endl;
  cout << "Channels: " << src.channels() << endl;
  cout << endl;

  cv::Mat blur;
  cv::blur(src, blur, cv::Size(3, 3));
  cout << "Blurred image ..." << endl;
  cout << "Dimensions: " << blur.rows << "x" << blur.cols << endl;
  cout << "Channels: " << blur.channels() << endl;
  cout << endl;

  double psnr = ipcv::Psnr(src, blur, 255);
  cout << "PSNR = " << psnr << endl;

  double rmse = ipcv::Rmse(src, blur);
  cout << "RMSE = " << rmse << endl;

  cv::Mat dE;
  int standard = 2000;
  string application = "graphic_arts";
  double deltae = ipcv::DeltaE(src, blur, dE, 255, standard, application);
  string label = "dE (" + to_string(standard) + ")";
  if (standard == 1994) {
    label += " (" + application + ")";
  }
  cout << label + " [avg] = " << deltae << endl;
  double dE_min, dE_max;
  cv::minMaxLoc(dE, &dE_min, &dE_max);
  cout << label + " [min] = " << dE_min << endl;
  cout << label + " [max] = " << dE_max << endl;
  cv::imshow(label, dE / dE_max);
  cv::waitKey(0);

  return EXIT_SUCCESS;
}
