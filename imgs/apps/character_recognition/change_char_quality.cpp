#include <ctime>
#include <iostream>
#include <random>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <tesseract/baseapi.h>
#include <tesseract/renderer.h>
#include <leptonica/allheaders.h>
#include <unordered_set>

#include "imgs/ipcv/geometric_transformation/GeometricTransformation.h"
#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace po = boost::program_options;
namespace {

//cv::Mat img_gamma_corrected;
//void gammaCorrection(const cv::Mat &img, const double gamma_)
//{
//    CV_Assert(gamma_ >= 0);
//    //! [changing-contrast-brightness-gamma-correction]
//    cv::Mat lookUpTable(1, 256, CV_8U);
//    uchar* p = lookUpTable.ptr();
//    for( int i = 0; i < 256; ++i)
//        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);
//
//    cv::Mat res = img.clone();
//    LUT(img, lookUpTable, res);
//    //! [changing-contrast-brightness-gamma-correction]
//
//    hconcat(img, res, img_gamma_corrected);
//    imshow("Gamma correction", img_gamma_corrected);
//}
}

int main(int argc, char* argv[]) {
    bool verbose = false;
    string template_directory = "";
    string dst_filename = "";
    string text_filename = "";


    po::options_description options("Options");
    options.add_options()("help,h", "display this message")(
      "verbose,v", po::bool_switch(&verbose), "verbose [default is silent]")(
      "template_directory,i", po::value<string>(&template_directory), "dataset folder name")
    ("dst-filename,o", po::value<string>(&dst_filename), "destination filename")
    ("text-filename,t", po::value<string>(&text_filename), "text filename");

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


    if (!boost::filesystem::is_directory(template_directory)) {
        cerr << "Provided template directory does not exists" << endl;
        return EXIT_FAILURE;
    }
    
    std::vector<cv::Mat> documentImages;
    std::vector<map<string, int>> textHistList;
    std::vector<float> totals;
    
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    
    
    
    for (boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(template_directory)) {
        string filename = entry.path().string();
//        cout << filename << endl;
        cv::Mat documentImg = cv::imread(filename, cv::IMREAD_GRAYSCALE);
//        documentImg.resize(sz);
        documentImages.push_back(documentImg);
        
        // Find the initial OCR histogram
        char cstr[filename.size()+1];
        strcpy(cstr, filename.c_str());
        Pix *alteredIm = pixRead(cstr);
        api->SetImage(alteredIm);
    //        outText = api->GetUTF8Text();
    //        printf("OCR output :\n%s", outText);
        api->Recognize(0);
        tesseract::ResultIterator* ri = api->GetIterator();
        tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
        
        map<string, int> textHist;
        
        if (ri != 0) {
            do {
                const char* symb = ri->GetUTF8Text(level);
                
                string character(symb);
    //                    float conf2 = ri2->Confidence(level2);
    //            printf("character: '%s';  \tconf: %.2f",
    //                   symb, conf2);
                textHist[character] += 1;
                
                delete[] symb;
            } while (ri->Next(level));
        }
        textHistList.push_back(textHist);
        
        float total = 0;
        for (pair<string, int> pair : textHist) {
                total += pair.second;
            }
        totals.push_back(total);
    }
    
    uchar threshold = 200;
    int nImages = 10;
    int nDocs = documentImages.size();
    
    cv::Mat map1, map2, dst;
    
    ofstream myfile;
    myfile.open(text_filename);
    int iterator = 1;
    double scale_x = 1;
    double scale_y = 1;
    double translation_x = 0;
    double translation_y = 0;
    double alpha = 0;
    double res = 0;
    double radians_per_degree = 3.14159265358979 / 180.0;
    
    map<string, int> test1, test2;
    string filename = "/Users/jakestevens/Documents/src/C++/examples/data/Scanned_documents/Picture_025.tif";
    char test1char[filename.size()+1];
    strcpy(test1char, filename.c_str());
    Pix *alteredImtest1 = pixRead(test1char);
    api->SetImage(alteredImtest1);
    api->Recognize(0);
    tesseract::ResultIterator* ri = api->GetIterator();
    tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
    if (ri != 0) {
        do {
            const char* symb = ri->GetUTF8Text(level);

            string character(symb);
            test1[character] += 1;

            delete[] symb;
        } while (ri->Next(level));
    }
    int total = 0;
    for (pair<string, int> pair : test1) {
        total += pair.second;
    }
    filename = "/Users/jakestevens/Desktop/test.png";
    char test2char[filename.size()+1];
    strcpy(test2char, filename.c_str());
    Pix *alteredImtest2 = pixRead(test2char);
    api->SetImage(alteredImtest2);
    api->Recognize(0);
    tesseract::ResultIterator* ri2 = api->GetIterator();
    tesseract::PageIteratorLevel level2 = tesseract::RIL_SYMBOL;
    if (ri != 0) {
        do {
            const char* symb = ri2->GetUTF8Text(level);

            string character(symb);
            cout << character << endl;
            test2[character] += 1;

            delete[] symb;
        } while (ri->Next(level2));
    }
    
    unordered_set<string> characters;
    for (pair<string, int> pair1 : test1) {
        characters.insert(pair1.first);
    }
    for (pair<string, int> OCRpair : test2) {
        characters.insert(OCRpair.first);
    }
    
    int errors = 0;
    for (string elem : characters) {
            int accuracy = abs(test1[elem]-test2[elem]);
            errors += accuracy;
        }
    
    // Calculate the total accuracy
    cout << total << endl;
    float totalAccuracy = (total-errors)/total;
//    if (totalAccuracy < 0.) {
//        totalAccuracy = 0.;
//    }
    string textAccuracy = to_string(totalAccuracy);
//    myfile << textAccuracy+"\n";
    cout << textAccuracy << endl;
    
//    for (int d = 0; d < nDocs; d++) {
//        for (int n = 1; n <= nImages; n++) {
//            cv::Mat src1 = documentImages[d].clone();
//            cv::Mat src2 = documentImages[d].clone();
//            cv::Mat src3 = documentImages[d].clone();
//            cv::Mat src4 = documentImages[d].clone();
//            cv::Mat src5 = documentImages[d].clone();
//            cv::Mat src6 = documentImages[d].clone();
//
//            if (src5.rows == 0 | src5.cols == 0) {
//                continue;
//            }
//
//            // Angle manipulations
//            bool status = true;
//            cv::Mat map1;
//            cv::Mat map2;
//            cv::Mat dst;
//            vector<cv::Point2f> ptsOut(4);
//
//            double theta = 0*radians_per_degree;
//            double phi = 0*radians_per_degree;
//            double psi = (65+2*n)*radians_per_degree;
//            status = ipcv::MapRotation3D(src2, map1, map2, ptsOut, theta, phi, psi);
//            cv::remap(src2, dst, map1, map2, cv::INTER_LINEAR);
////            cv::imshow("testy", dst);
//
//            vector<cv::Point2f> ptsIn(4);
//            ptsIn[0].x = 0;
//            ptsIn[0].y = src2.rows - 1;
//            ptsIn[1].x = src2.cols - 1;
//            ptsIn[1].y = src2.rows - 1;
//            ptsIn[2].x = src2.cols - 1;
//            ptsIn[2].y = 0;
//            ptsIn[3].x = 0;
//            ptsIn[3].y = 0;
//
//            status = ipcv::MapQ2Q(dst, src2, ptsOut, ptsIn, map1, map2);
//            cv::remap(dst, dst, map1, map2, cv::INTER_LINEAR);
////            cv::imshow("test", dst);
////            cv::waitKey();
//
//
////            alpha = 0.15-n*0.01;
////            res = 0.5-n*0.03;
////            alpha = 0.25;
//
//            // Change the resolution
////            cv::resize(src5, src5, cv::Size(), res, res);
//            // Change the contrast
////            src4.convertTo(src4, -1, alpha, 0);
////            cv::Mat img_gamma_corrected(src4.rows, src4.cols*2, src4.type());
////            double gamma_ = 0.0001+0.1*(n+1);
////            //! [changing-contrast-brightness-gamma-correction]
////            cv::Mat lookUpTable(1, 256, CV_8UC1);
////            uchar* p = lookUpTable.ptr();
////
////            for (int i = 0; i < 256; i++) {
////                p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);
////            }
////
////            cv::LUT(src4, lookUpTable, src4);
////
////            cv::hconcat(documentImages[d], src4, img_gamma_corrected);
////            cv::imshow("Gamma correction", img_gamma_corrected);
////            cv::waitKey(0);
////
//
//            // Change the focus of the image
////            cv::blur(src6, src6, cv::Size(2*n+1,2*n+1));
////            cv::hconcat(documentImages[d], src4, img_gamma_corrected);
////            cv::imshow("Gamma correction", img_gamma_corrected);
////            cv::waitKey(0);
////
//
////             Create and add noise to the image
////            cv::Mat noise(src3.size(), CV_32FC1);
////            float sigma = 0.03*n;
////            cv::randn(noise, 0, sigma);
////            noise *= 255;
////            cout << noise << endl;
//            //convert to float first
////            src3.convertTo(src3, CV_32F);
////            src3 += noise;
////            src3.convertTo(src3, CV_8U);
////            cv::imshow("noisey image", src3);
////            cv::waitKey(0);
//
//
////
////            for (int i = 0; i < src1.rows; i++) {
////                for (int j = 0; j < src1.cols; j++) {
////                    if (src1.at<uchar>(i,j) <= threshold) {
////                        bool TrueFalse = (rand() % 100) < 100-(1*(n+1));
////                        if (!TrueFalse) {
////                            src1.at<uchar>(i,j) = 255;
////                        }
////                    }
////                }
////            }
////
//
//            string num = to_string(iterator++);
//            if (iterator <= 10) {
//                num.insert(0, "0");
//            }
////            string new_char_dst_filename = dst_filename+num+".tif";
////            num = to_string(interator++);
//            string new_angle_dst_filename = dst_filename+num+".tif";
////            num = to_string(interator++);
////            string new_noise_dst_filename = dst_filename+num+".tif";
////            num = to_string(interator++);
////            string new_contrast_dst_filename = dst_filename+num+".tif";
////            num = to_string(interator++);
////            string new_res_dst_filename = dst_filename+num+".tif";
////            num = to_string(interator++);
////            string new_blur_dst_filename = dst_filename+num+".tif";
////            cv::imwrite(new_char_dst_filename, src1);
//            cv::imwrite(new_angle_dst_filename, dst);
////            cv::imwrite(new_noise_dst_filename, src3);
////            cv::imwrite(new_contrast_dst_filename, src4);
////            cv::imwrite(new_res_dst_filename, src5);
////            cv::imwrite(new_blur_dst_filename, src6);
////            cout << new_contrast_dst_filename << endl;
////            cv::imwrite(new_res_dst_filename, src5);
////            char cstr1[new_char_dst_filename.size()+1];
//            char cstr2[new_angle_dst_filename.size()+1];
////            char cstr3[new_noise_dst_filename.size()+1];
////            char cstr4[new_contrast_dst_filename.size()+1];
////            char cstr5[new_res_dst_filename.size()+1];
////            char cstr6[new_blur_dst_filename.size()+1];
////            strcpy(cstr1, new_char_dst_filename.c_str());
//            strcpy(cstr2, new_angle_dst_filename.c_str());
////            strcpy(cstr3, new_noise_dst_filename.c_str());
////            strcpy(cstr4, new_contrast_dst_filename.c_str());
////            strcpy(cstr5, new_res_dst_filename.c_str());
////            strcpy(cstr6, new_blur_dst_filename.c_str());
////            Pix *alteredIm1 = pixRead(cstr1);
//            Pix *alteredIm2 = pixRead(cstr2);
////            Pix *alteredIm3 = pixRead(cstr3);
////            Pix *alteredIm4 = pixRead(cstr4);
////            Pix *alteredIm5 = pixRead(cstr5);
////            Pix *alteredIm6 = pixRead(cstr6);
////            api->SetImage(alteredIm1);
////            api->Recognize(0);
////            tesseract::ResultIterator* ri = api->GetIterator();
////            tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
//            map<string, int> OCRtextHist1, OCRtextHist2, OCRtextHist3, OCRtextHist4, OCRtextHist5, OCRtextHist6;
////            if (ri != 0) {
////                do {
////                    const char* symb = ri->GetUTF8Text(level);
////
////                    string character(symb);
////                    OCRtextHist1[character] += 1;
////
////                    delete[] symb;
////                } while (ri->Next(level));
////            }
//            api->SetImage(alteredIm2);
//            api->Recognize(0);
//            tesseract::ResultIterator* ri = api->GetIterator();
//            tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
//            if (ri != 0) {
//                do {
//                    const char* symb = ri->GetUTF8Text(level);
//
//                    string character(symb);
//                    OCRtextHist2[character] += 1;
//
//                    delete[] symb;
//                } while (ri->Next(level));
//            }
////            api->SetImage(alteredIm3);
////            api->Recognize(0);
////            tesseract::ResultIterator* ri = api->GetIterator();
////            tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
////
////            if (ri != 0) {
////                do {
////                    const char* symb = ri->GetUTF8Text(level);
////
////                    string character(symb);
////    //                    float conf2 = ri2->Confidence(level2);
////        //            printf("character: '%s';  \tconf: %.2f",
////        //                   symb, conf2);
////                    OCRtextHist3[character] += 1;
////
////                    delete[] symb;
////                } while (ri->Next(level));
////            }
//
////            api->SetImage(alteredIm4);
////            api->Recognize(0);
////            tesseract::ResultIterator* ri = api->GetIterator();
////            tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
////            if (ri != 0) {
////                do {
////                    const char* symb = ri->GetUTF8Text(level);
////
////                    string character(symb);
////                    OCRtextHist4[character] += 1;
////
////                    delete[] symb;
////                } while (ri->Next(level));
////            }
//
////            api->SetImage(alteredIm5);
////            api->Recognize(0);
//////            ri = api->GetIterator();
//////            level = tesseract::RIL_SYMBOL;
////            tesseract::ResultIterator* ri = api->GetIterator();
////            tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
////            if (ri != 0) {
////                do {
////                    const char* symb = ri->GetUTF8Text(level);
////
////                    string character(symb);
////    //                    float conf2 = ri2->Confidence(level2);
////        //            printf("character: '%s';  \tconf: %.2f",
////        //                   symb, conf2);
////                    OCRtextHist5[character] += 1;
////
////                    delete[] symb;
////                } while (ri->Next(level));
////            }
//
////            api->SetImage(alteredIm6);
////            api->Recognize(0);
////            tesseract::ResultIterator* ri = api->GetIterator();
////            tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
////
////            if (ri != 0) {
////                do {
////                    const char* symb = ri->GetUTF8Text(level);
////                    string character(symb);
////                    OCRtextHist6[character] += 1;
////                    delete[] symb;
////                } while (ri->Next(level));
////            }
//
//
//            float errors = 0.;
//            map<string, int>& textHist = textHistList[d];
////            for (pair<string, int> pair : OCRtextHist1) {
////    //            cout << pair.first << ": " << pair.second << endl;
////                int accuracy = textHist[pair.first]-pair.second;
////                if (accuracy != 0) {
////                    errors += abs(accuracy);
////                }
////            }
////
//            float total = totals[d];
////            float totalAccuracy = (total-errors)/total;
////            string textAccuracy = to_string(totalAccuracy);
////            myfile << textAccuracy+"\n";
//////            cout << textAccuracy << endl;
////            float totalAccuracy = (total-errors)/total;
////            string textAccuracy = to_string(totalAccuracy);
////            myfile << textAccuracy+"\n";
////
////            errors = 0.;
//
//            // Create a set of all the characters from all frequency dictionaries
//            unordered_set<string> characters;
//            for (pair<string, int> pair1 : textHist) {
//                characters.insert(pair1.first);
//            }
//            for (pair<string, int> OCRpair : OCRtextHist1) {
//                characters.insert(OCRpair.first);
//            }
//            for (pair<string, int> OCRpair : OCRtextHist2) {
//                characters.insert(OCRpair.first);
//            }
//            for (pair<string, int> OCRpair : OCRtextHist3) {
//                characters.insert(OCRpair.first);
//            }
//            for (pair<string, int> OCRpair : OCRtextHist4) {
//                characters.insert(OCRpair.first);
//            }
//            for (pair<string, int> OCRpair : OCRtextHist5) {
//                characters.insert(OCRpair.first);
//            }
//            for (pair<string, int> OCRpair : OCRtextHist6) {
//                characters.insert(OCRpair.first);
//            }
//
//
//            // Calculate the character accuracy for each character and update the error count
////            for (string elem : characters) {
////                int accuracy = abs(textHist[elem]-OCRtextHist1[elem]);
////                errors += accuracy;
////            }
//
////            for (pair<string, int> pair : textHist) {
////                int error = abs(pair.second-OCRtextHist1[pair.first]);
////                errors += error;
////            }
//            // Calculate the mean character accuracy
////            float totalAccuracy = (total-errors)/total;
////            if (totalAccuracy < 0.) {
////                totalAccuracy = 0.;
////            }
////            string textAccuracy = to_string(totalAccuracy);
////            myfile << textAccuracy+"\n";
////            cout << textAccuracy << endl;
//
////            for (string elem : characters) {
////                int accuracy = abs(textHist[elem]-OCRtextHist3[elem]);
////                errors += accuracy;
////            }
////            // Calculate the total accuracy
////            float totalAccuracy = (total-errors)/total;
////            if (totalAccuracy < 0.) {
////                totalAccuracy = 0.;
////            }
////            string textAccuracy = to_string(totalAccuracy);
////            myfile << textAccuracy+"\n";
////            cout << textAccuracy << endl;
//
//            for (string elem : characters) {
//                int accuracy = abs(textHist[elem]-OCRtextHist2[elem]);
//                errors += accuracy;
//            }
//
////            for (string elem : characters) {
////                int accuracy = abs(textHist[elem]-OCRtextHist4[elem]);
////                errors += accuracy;
////            }
//
////            for (string elem : characters) {
////                int accuracy = abs(textHist[elem]-OCRtextHist5[elem]);
////                errors += accuracy;
////            }
//
////            for (string elem : characters) {
////                int accuracy = abs(textHist[elem]-OCRtextHist6[elem]);
////                errors += accuracy;
////            }
//            // Calculate the total accuracy
//            float totalAccuracy = (total-errors)/total;
//            if (totalAccuracy < 0.) {
//                totalAccuracy = 0.;
//            }
//            string textAccuracy = to_string(totalAccuracy);
//            myfile << textAccuracy+"\n";
//            cout << textAccuracy << endl;
////
//////            // Calculate the total accuracy
////            float totalAccuracy = (total-errors)/total;
////            if (totalAccuracy < 0.) {
////                totalAccuracy = 0.;
////            }
////            string textAccuracy = to_string(totalAccuracy);
////            myfile << textAccuracy+"\n";
////            cout << textAccuracy << endl;
//
////            for (string elem : characters) {
////                int accuracy = abs(textHist[elem]-OCRtextHist5[elem]);
////                errors += accuracy;
////            }
////            // Calculate the total accuracy
////            float totalAccuracy = (total-errors)/total;
////            if (totalAccuracy < 0.) {
////                totalAccuracy = 0.;
////            }
////            string textAccuracy = to_string(totalAccuracy);
////            myfile << textAccuracy+"\n";
////            cout << textAccuracy << endl;
//
//
//
//        //     Destroy used object and release memory
////            pixDestroy(&alteredIm1);
//            pixDestroy(&alteredIm2);
////            pixDestroy(&alteredIm6);
////            pixDestroy(&alteredIm4);
////            pixDestroy(&alteredIm5);
//        }
//    }
    
    api->End();
    
//    myfile.close();

  return EXIT_SUCCESS;
}
