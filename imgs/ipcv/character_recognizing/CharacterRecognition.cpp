#include "CharacterRecognition.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <map>

using namespace std;

void Spatial(const cv::Mat& src, std::string& text, cv::Mat& h, const std::vector<cv::Mat>& templates, const std::vector<int>& codes, const double threshold) {

    cv::Mat src_gray = src;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    h = cv::Mat_<int>::zeros(1, templates.size());
    int idx = 0;
    map<int, int> textMap;
    for (cv::Mat character : templates) {
        bitwise_not(character, character);
        character.convertTo(character, CV_32F);
        if (cv::sum(character)[0] != 0) {
            character /= cv::sum(character)[0];
        }
        cv::Mat correlationIm;
        cv::filter2D(src_gray, correlationIm, CV_32F, character);
        cv::threshold(correlationIm, correlationIm, threshold, 1, cv::THRESH_BINARY_INV);
        vector<cv::Point> characterCoors;
        cv::findNonZero(correlationIm, characterCoors);
        for (cv::Point coor : characterCoors) {
            textMap[coor.y*src.cols+coor.x] = codes[idx];
        }
        h.at<int>(0, idx++) = characterCoors.size();
        
    }
    for (pair<int, int> pair : textMap) {
        text += static_cast<wchar_t>(pair.second);
    }
}

void Matched(const cv::Mat& src, std::string& text, cv::Mat& h, const std::vector<cv::Mat>& templates, const std::vector<int>& codes, const double threshold) {
    
    cv::Size tsize = templates[0].size();
    
    cv::Mat src_gray = src;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    h = cv::Mat_<int>::zeros(1, templates.size());
    int idx = 0;
    map<int, int> textMap;
    for (cv::Mat character : templates) {
        cv::Mat flatChar = character.reshape(1);
        vector<cv::Point> characterCoors;
        for (int i = tsize.height; i < src.rows-tsize.height+1; i++) {
            for (int j = tsize.width; j < src.cols-tsize.width+1; j++) {
                cv::Rect roi(j, i, tsize.width, tsize.height);
                cv::Mat cut = src_gray(roi).clone();
                cv::Mat flatCut = cut.reshape(1);
                double numer = flatChar.dot(flatCut);
                double denom = cv::norm(flatChar)*cv::norm(flatCut);
                double angle = 0;
                if (denom != 0) {
                    angle = numer/denom;
                }
                else{
                    angle = 1;
                }
                if (angle >= threshold) {
                    characterCoors.push_back(cv::Point(j,i));
                }
            }
        }
        for (cv::Point coor : characterCoors) {
            textMap[coor.y*src.cols+coor.x] = codes[idx];
        }
        h.at<int>(0, idx++) = characterCoors.size();
    }
    for (pair<int, int> pair : textMap) {
        text += static_cast<wchar_t>(pair.second);
    }
}

namespace ipcv {

bool CharacterRecognition(const cv::Mat& src, std::string& text, cv::Mat& h, const std::vector<cv::Mat>& templates, const std::vector<int>& codes, const double threshold, const RecognitionType recognition_type) {

  switch (recognition_type) {
    case RecognitionType::spatial:
      Spatial(src, text, h, templates, codes, threshold);
      break;
    case RecognitionType::matched:
      Matched(src, text, h, templates, codes, threshold);
      break;
    default:
      cerr << "Specified recognition type is unsupported" << endl;
      return false;
  }

  return true;
}
}
