#include <ctime>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "imgs/ipcv/character_recognizing/CharacterRecognition.h"

using namespace std;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    bool verbose = false;
    double threshold = 0.999999;
    string src_filename = "";
    string template_directory = "";

    string recognition_type_string = "matched";
    ipcv::RecognitionType recognition_type;

    po::options_description options("Options");
    options.add_options()("help,h", "display this message")(
      "verbose,v", po::bool_switch(&verbose), "verbose [default is silent]")(
      "source-filename,i", po::value<string>(&src_filename), "source filename")(
      "template_directory,d", po::value<string>(&template_directory),
      "template directory")("threshold,t",
                              po::value<double>(&threshold),
                              "threshold [default is 0.999999]")(
      "recognition-type,r", po::value<string>(&recognition_type_string),
      "recognition type (spatial | matched) [default is spatial]");

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

    if (recognition_type_string == "spatial") {
        recognition_type = ipcv::RecognitionType::spatial;
    } else if (recognition_type_string == "matched") {
        recognition_type = ipcv::RecognitionType::matched;
    } else {
        cerr << "Provided recognition type is not supported" << endl;
        return EXIT_FAILURE;
    }

    if (!boost::filesystem::exists(src_filename)) {
        cerr << "Provided source file does not exists" << endl;
        return EXIT_FAILURE;
    }

    if (!boost::filesystem::is_directory(template_directory)) {
        cerr << "Provided template directory does not exists" << endl;
        return EXIT_FAILURE;
    }

    cv::Mat src = cv::imread(src_filename, cv::IMREAD_COLOR);


    std::vector<cv::Mat> characterImages;
    std::vector<int> characterCodes;

    for (boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(template_directory)) {
        string character = entry.path().string();
        int code = stoi(entry.path().stem().string());
        cv::Mat characterIm = cv::imread(character, cv::IMREAD_UNCHANGED);
        characterImages.push_back(characterIm);
        characterCodes.push_back(code);
    }

    if (verbose) {
        cout << "Source filename: " << src_filename << endl;
        cout << "Template directory: " << template_directory << endl;
        cout << "Size: " << src.size() << endl;
        cout << "Channels: " << src.channels() << endl;
        cout << "Threshold: " << threshold << endl;
        cout << "Recognition type: " << recognition_type_string << endl;
    }

    std::string text;
    cv::Mat h;

    clock_t startTime = clock();
    ipcv::CharacterRecognition(src, text, h, characterImages, characterCodes, threshold, recognition_type);
    clock_t endTime = clock();

    cout << h << endl;
    cout << text << endl;

    if (verbose) {
    cout << "Elapsed time: "
         << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
         << " [s]" << endl;
    }

    return EXIT_SUCCESS;
}
