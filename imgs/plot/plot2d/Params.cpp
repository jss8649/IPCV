/** Implementation file for plot parameter specification
 *
 *  \file plot/plot2d/Params.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 23 Mar 2018
 */

#include "Params.h"

#include <iostream>

using namespace std;

namespace plot::plot2d {

Params::Params() {}

Params::Params(std::string x_label, std::string y_label) {
  x_label_ = x_label;
  y_label_ = y_label;
}

Params::~Params() {}

std::string Params::get_x_label() { return x_label_; }
void Params::set_x_label(std::string x_label) { x_label_ = x_label; }

std::string Params::get_y_label() { return y_label_; }
void Params::set_y_label(std::string y_label) { y_label_ = y_label; }

std::string Params::get_title() { return title_; }
void Params::set_title(std::string title) { title_ = title; }

double Params::get_x_min() { return x_min_; }
void Params::set_x_min(double x_min) { x_min_ = x_min; }
double Params::get_x_max() { return x_max_; }
void Params::set_x_max(double x_max) { x_max_ = x_max; }

double Params::get_y_min() { return y_min_; }
void Params::set_y_min(double y_min) { y_min_ = y_min; }
double Params::get_y_max() { return y_max_; }
void Params::set_y_max(double y_max) { y_max_ = y_max; }

bool Params::get_x_tics() { return x_tics_; }
void Params::set_x_tics(bool x_tics) { x_tics_ = x_tics; }
bool Params::get_y_tics() { return y_tics_; }
void Params::set_y_tics(bool y_tics) { y_tics_ = y_tics; }

double Params::get_xvline() { return xvline_; }
void Params::set_xvline(double xvline) { xvline_ = xvline; }

std::string Params::get_destination_filename() { return dst_filename_; }
void Params::set_destination_filename(std::string dst_filename) {
  dst_filename_ = dst_filename;
}

void Params::Print() {
  cout << "Params dump:" << endl;
  cout << "  x_label: " << x_label_ << endl;
  cout << "  y_label: " << y_label_ << endl;
  cout << "  title: " << title_ << endl;
  cout << "  x_min: " << x_min_ << endl;
  cout << "  x_max: " << x_max_ << endl;
  if (x_min_ >= x_max_) {
    cout << "  *** Autoscaling being used for the x-axis" << endl;
  }
  cout << "  y_min: " << y_min_ << endl;
  cout << "  y_max: " << y_max_ << endl;
  if (y_min_ >= y_max_) {
    cout << "  *** Autoscaling being used for the y-axis" << endl;
  }
  cout << "  x_tics: " << x_tics_ << endl;
  cout << "  y_tics: " << y_tics_ << endl;
  cout << "  xvline: " << xvline_ << endl;
  cout << "  dst_filename: " << dst_filename_ << endl;
}
}
