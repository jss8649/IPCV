#include <ctime>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    bool verbose = false;
    string home = getenv("HOME");

    string src_filename = "";
    string dst_filename = "";

    po::options_description options("Options");
    options.add_options()("help,h", "display this message")(
      "verbose,v", po::bool_switch(&verbose), "verbose [default is silent]")(
      "source-filename,i", po::value<string>(&src_filename), "source filename")(
      "destination-filename,o", po::value<string>(&dst_filename),
      "destination filename");

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

    if (verbose) {
    cout << "Source filename: " << src_filename << endl;
    cout << "Size: " << src.size() << endl;
    cout << "Channels: " << src.channels() << endl;
    cout << "Destination filename: " << dst_filename << endl;
    }
    
    cv::Mat dft_output = cv::Mat::zeros(src.size(), CV_64F);
    cv::Mat planes[2] = {cv::Mat_<double>(src).clone(), cv::Mat::zeros(src.size(), CV_64F)};
    cv::merge(planes, 2, dft_output);
    cv::dft(dft_output, dft_output);
    cv::split(dft_output, planes);
    
    cv::Mat mags = ipcv::DftMagnitude(dft_output, ipcv::DFT_MAGNITUDE_LOG +
                                                  ipcv::DFT_MAGNITUDE_NORMALIZE);
    cv::Mat log_mag_DFT = ipcv::DftShift(mags);
    log_mag_DFT *= 255;
    log_mag_DFT.convertTo(log_mag_DFT, src.type());

    mags = mags.reshape(1,1);
    cv::Mat sort;
    cv::sortIdx(mags, sort, cv::SORT_DESCENDING);

    cout << dft_output.size << endl;

    clock_t startTime = clock();
    cv::Mat used = cv::Mat::zeros(src.size(), CV_8U);
    uchar* usedPtr = used.ptr<uchar>();
    int* sortPtr = sort.ptr<int>();
    double* magsPtr = mags.ptr<double>();
    double* realPtr = planes[0].ptr<double>();
    double* imagPtr = planes[1].ptr<double>();
    cv::Mat frames[2] = {cv::Mat::zeros(src.size(), CV_64F), cv::Mat::zeros(src.size(), CV_64F)};
    double* frame1Ptr = frames[0].ptr<double>();
    double* frame2Ptr = frames[1].ptr<double>();
    double mag;
    bool pause = false;

    cv::VideoWriter video_writer;
    cv::Size frameSize(src.cols*3,src.rows*2);
    video_writer.open(dst_filename, cv::CAP_FFMPEG, cv::VideoWriter::fourcc('m','p','4','v'), 24.,
                      frameSize);

    for (int i = 0; i < sort.cols; i++) {
        cv::Mat CC, CCscaled, frame;
        cv::Mat CCtmp[2] = {cv::Mat::zeros(src.size(), CV_64F), cv::Mat::zeros(src.size(), CV_64F)};
        double* CC1Ptr = CCtmp[0].ptr<double>();
        double* CC2Ptr = CCtmp[1].ptr<double>();
        mag = magsPtr[sortPtr[i]];
        cout << mag << endl;
        usedPtr[sortPtr[i]] = mag*255;
        CC1Ptr[sortPtr[i]] = frame1Ptr[sortPtr[i]] = realPtr[sortPtr[i]];
        CC2Ptr[sortPtr[i]] = frame2Ptr[sortPtr[i]] = imagPtr[sortPtr[i]];

        cv::Mat invPlanes[2];
        cv::merge(CCtmp, 2, CC);
        cv::dft(CC, CC, cv::DFT_INVERSE);
        cv::split(CC, invPlanes);
        invPlanes[0].convertTo(CC, src.type());
        CCscaled = mag*CC;

        cv::merge(frames, 2, frame);
        cv::dft(frame, frame, cv::DFT_INVERSE + cv::DFT_SCALE);
        cv::split(frame, invPlanes);
        invPlanes[0].convertTo(frame, src.type());

        cv::Mat usedDisp = ipcv::DftShift(used);

        cv::imshow("Original Image", src);
        cv::imshow("Fourier Transform - log(magnitude)", log_mag_DFT);
        cv::imshow("Current Component", CC);
        cv::imshow("Current Component (Scaled)", CCscaled);
        cv::imshow("Summed Components", frame);
        cv::imshow("Fourier Coefficients Used - log(magnitude)", usedDisp);

        cv:: Mat top, bot, videoFrame;
        cv::hconcat(src.clone(), log_mag_DFT, top);
        cv::hconcat(top, CC, top);
        cv::hconcat(CCscaled, frame, bot);
        cv::hconcat(bot, usedDisp, bot);
        cv::vconcat(top, bot, videoFrame);
        
        cv::imshow("All Windows", videoFrame);

        // Key controls
        int key = cv::waitKey(1);
        if(key == 80; key == 112){
            pause = true;
            while (pause) {
                key = cv::waitKey(0);
                if(key == 80; key == 112){
                    pause = false;
                }
                else if (key == 27){
                    break;
                }
            }
        }
        else if (key == 27){
            break;
        }
    }

    clock_t endTime = clock();

    if (verbose) {
    cout << "Elapsed time: "
         << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
         << " [s]" << endl;
    }

    return EXIT_SUCCESS;
    }

