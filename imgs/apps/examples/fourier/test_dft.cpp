#include <complex>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

int main() {
  // Set up parameters for 1-dimensional DFT testing
  int N = 32;
  int repetitions = 100000;

//   Create and display the original 1-dimensional function using a
//   vector of double-precision complex values
  vector<complex<double>> f(N);
  for (int idx = 0; idx < N; idx++) {
    f[idx] = 1;
  }
  cout << "Original function:" << endl;
  for (int idx = 0; idx < N; idx++) {
    cout << "f[" << idx << "]: " << f[idx] << endl;
  }
  cout << endl;

//  // Perform 1-dimensional DFT and time multiple repetitions
  int flag = ipcv::DFT_SCALE;
  vector<complex<double>> F = ipcv::Dft(f, flag);
  cout << "Fourier transform:" << endl;
  for (int idx = 0; idx < N; idx++) {
    cout << "F[" << idx << "]: " << F[idx] << endl;
  }
  clock_t startTime = clock();
  for (int idx = 0; idx < repetitions; idx++) {
    F = ipcv::Dft(f, flag);
  }
  clock_t endTime = clock();
  cout << "Elapsed time: "
       << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
       << " [s] for "
       << repetitions
       << " repetitions"
       << endl;
  cout << endl;

//  // Perform 1-dimensional inverse DFT and time multiple repititions
  flag = ipcv::DFT_INVERSE;
  f = ipcv::Dft(F, flag);
  cout << "Inverse Fourier transform:" << endl;
  for (int idx = 0; idx < N; idx++) {
    cout << "f[" << idx << "]: " << f[idx] << endl;
  }
  startTime = clock();
  for (int idx = 0; idx < repetitions; idx++) {
    f = ipcv::Dft(F, flag);
  }
  endTime = clock();
  cout << "Elapsed time: "
       << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
       << " [s] for "
       << repetitions
       << " repetitions"
       << endl;
  cout << endl;

  // Create and display the original 1-dimensional function using cv::Mat
  cv::Mat f_Mat = cv::Mat::ones(N, 1, CV_64F);
  cout << "Original function (cv::Mat):" << endl;
  cout << "f:\n" << f_Mat << endl;
  cout << endl;

  // Perform 1-dimensional DFT and time multiple repititions (OpenCV)
  cv::Mat F_Mat;
  cv::dft(f_Mat, F_Mat, cv::DFT_SCALE);
  cout << "Fourier transform (OpenCV):" << endl;
  cout << "F:\n" << F_Mat << endl;
   startTime = clock();
  for (int idx = 0; idx < repetitions; idx++) {
    cv::dft(f_Mat, F_Mat, cv::DFT_SCALE);
  }
   endTime = clock();
  cout << "Elapsed time: "
       << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
       << " [s] for "
       << repetitions
       << " repetitions"
       << endl;
  cout << endl;

  // Perform 1-dimensional inverse DFT and time multiple repititions (OpenCV)
  cv::dft(F_Mat, f_Mat, cv::DFT_INVERSE);
  cout << "Inverse Fourier transform (OpenCV):" << endl;
  cout << "f:\n" << f_Mat << endl;
  startTime = clock();
  for (int idx = 0; idx < repetitions; idx++) {
    cv::dft(F_Mat, f_Mat, cv::DFT_INVERSE);
  }
  endTime = clock();
  cout << "Elapsed time: "
       << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
       << " [s] for "
       << repetitions
       << " repetitions"
       << endl;
  cout << endl;

  // Read image in to cv::Mat
  string filename = "/Users/jakestevens/Documents/src/C++/examples/data/lenna.tif";
  cv::Mat src;
  src = cv::imread(filename, cv::IMREAD_UNCHANGED);
  cout << "Image filename:" << endl;
  cout << filename << endl;
  cout << endl;

  // Perform 2-dimensional DFT and time execution (OpenCV)
  cv::Mat dft_input = src.clone();
  cv::Mat dft_output;
  dft_input.convertTo(dft_input, CV_64F);
  startTime = clock();
  cv::dft(dft_input, dft_output, cv::DFT_SCALE);
  endTime = clock();
  cout << "Fourier transform (OpenCV):" << endl;
  cout << "Elapsed time: "
       << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
       << " [s]"
       << endl;
  cout << endl;

  // Perform 2-dimensional inverse DFT and time execution (OpenCV)
  cv::Mat idft_output;
  startTime = clock();
  cv::dft(dft_output, idft_output, cv::DFT_INVERSE);
  endTime = clock();
  cout << "Inverse Fourier transform (OpenCV):" << endl;
  cout << "Elapsed time: "
       << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
       << " [s]"
       << endl;
  cout << endl;

  // Report out diagnotic values for the 2-dimension DFT runs
  double src_min;
  double src_max;
  cv::minMaxLoc(src, &src_min, &src_max);

  double dft_output_min;
  double dft_output_max;
  cv::minMaxLoc(dft_output, &dft_output_min, &dft_output_max);

  double idft_output_min;
  double idft_output_max;
  cv::minMaxLoc(idft_output, &idft_output_min, &idft_output_max);

  int number_pixels = src.rows * src.cols;
  cout << "Number of pixels: " << number_pixels << endl;
  cout << "Source (mean): " << cv::mean(src).val[0] << endl;
  cout << "Source (size): " << src.size() << endl;
  cout << "Source (channels): " << src.channels() << endl;
  cout << "Source (type): " << src.type() << endl;
  cout << "Source (min): " << src_min << endl;
  cout << "Source (max): " << src_max << endl;
  cout << "DFT [scaled] (size): " << dft_output.size() << endl;
  cout << "DFT [scaled] (channels): " << dft_output.channels() << endl;
  cout << "DFT [scaled] (type): " << dft_output.type() << endl;
  cout << "DFT [scaled] (min): " << dft_output_min << endl;
  cout << "DFT [scaled] (max): " << dft_output_max << endl;
  cout << "iDFT (size): " << idft_output.size() << endl;
  cout << "iDFT (channels): " << idft_output.channels() << endl;
  cout << "iDFT (type): " << idft_output.type() << endl;
  cout << "iDFT (min): " << idft_output_min << endl;
  cout << "iDFT (max): " << idft_output_max << endl;

  // Display the original image, the log|DFT|, and the inverse DFT
  idft_output.convertTo(idft_output, src.type());
  cv::imshow("Source", src);
  cv::imshow("log|DFT|",
             ipcv::DftMagnitude(dft_output,
                                ipcv::DFT_MAGNITUDE_LOG +
                                ipcv::DFT_MAGNITUDE_CENTER +
                                ipcv::DFT_MAGNITUDE_NORMALIZE));
  cv::imshow("iDFT", idft_output);
  cv::waitKey(0);

  return EXIT_SUCCESS;
}
