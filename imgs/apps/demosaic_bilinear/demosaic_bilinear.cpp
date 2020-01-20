#include <ctime>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "imgs/ipcv/demosaicing/Demosaic.h"
#include "imgs/ipcv/utils/Utils.h"

#include "imgs/ipcv/histogram_enhancement/LinearLut.h"

using namespace std;
//using namespace ;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  bool verbose = false;
  string src_filename = "";
  string dst_filename = "";
  string pattern = "GRBG";

  po::options_description options("Options");
  options.add_options()("help,h", "display this message")(
      "verbose,v", po::bool_switch(&verbose), "verbose [default is silent]")(
      "source-filename,i", po::value<string>(&src_filename), "source filename")(
      "destination-filename,o", po::value<string>(&dst_filename),
      "destination filename")("pattern,p", po::value<string>(&pattern),
                              "pattern [default is GBRG]");

  po::positional_options_description positional_options;
  positional_options.add("source-filename", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
                .options(options)
                .positional(positional_options)
                .run(),
            vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << "Usage: " << argv[0] << " [options] source-filename" << endl;
    cout << options << endl;
    return EXIT_SUCCESS;
  }

  if (!boost::filesystem::exists(src_filename)) {
    cerr << "Provided source file does not exists" << endl;
    return EXIT_FAILURE;
  }

  cv::Mat src = cv::imread(src_filename, cv::IMREAD_UNCHANGED);
  cv::Mat ref = cv::imread("/Users/jakestevens/Documents/src/python/examples/data/demosaic/demosaickTest.tif", cv::IMREAD_COLOR);

  if (verbose) {
    cout << "Source filename: " << src_filename << endl;
    cout << "Size: " << src.size() << endl;
    cout << "Channels: " << src.channels() << endl;
    cout << "Pattern: " << pattern << endl;
    cout << "Destination filename: " << dst_filename << endl;
  }

  clock_t startTime = clock();

  cv::Mat dst1 = ipcv::Homogeneity(src, pattern);
    
    clock_t endTime = clock();
    double t1 = endTime - startTime;
    startTime = clock();
  cv::Mat dst2 = ipcv::Bilinear(src, pattern);
     endTime = clock();
    double t2 = endTime - startTime;
     startTime = clock();
  cv::Mat dst3 = ipcv::LarochePrescott(src, pattern);
     endTime = clock();
    double t3 = endTime - startTime;
  dst1.convertTo(dst1, CV_8U);
  dst2.convertTo(dst2, CV_8U);
  dst3.convertTo(dst3, CV_8U);
    
    double psnr1 = ipcv::Psnr(dst1, ref, 255);
    double psnr2 = ipcv::Psnr(dst2, ref, 255);
    double psnr3 = ipcv::Psnr(dst3, ref, 255);
    double rmse1 = ipcv::Rmse(dst1, ref);
    double rmse2 = ipcv::Rmse(dst2, ref);
    double rmse3 = ipcv::Rmse(dst3, ref);
    
    cout << psnr1 << "," << psnr2 << "," << psnr3 << endl;
    cout << rmse1 << "," << rmse2 << "," << rmse3 << endl;

  if (verbose) {
    cout << "Elapsed time: "
        << (t1) / static_cast<double>(CLOCKS_PER_SEC)
      << " [s]" << endl;
      cout << (t2) / static_cast<double>(CLOCKS_PER_SEC)
      << " [s]" << endl;
      cout << (t3) / static_cast<double>(CLOCKS_PER_SEC)
         << " [s]" << endl;
  }

  if (dst_filename.empty()) {
    cv::imshow(src_filename, src);
    cv::imshow(src_filename + " [CFA Interpolated]", dst1);
    cv::waitKey(0);
    cv::imwrite("/Users/jakestevens/Documents/src/C++/examples/data/demosaickHomo.tif", dst1);
    cv::imwrite("/Users/jakestevens/Documents/src/C++/examples/data/demosaickBilinear.tif", dst2);
    cv::imwrite("/Users/jakestevens/Documents/src/C++/examples/data/demosaickLaroche.tif", dst3);
  } else {
//    cv::imwrite(dst_filename, dst);
  }
    

  return EXIT_SUCCESS;
}
