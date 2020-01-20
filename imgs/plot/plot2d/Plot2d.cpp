/** Implementation file for 2D plotting
 *
 *  \file imgs/plot/plot2d/Plot2d.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 28 Aug 2018
 */

#include "Plot2d.h"

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

#include "imgs/third_party/gnuplot-iostream.h"
#include "imgs/plot/plot2d/Params.h"

using namespace std;

void set_common_parameters(string& gp_msg, plot::plot2d::Params params) {
  if (params.get_destination_filename() == "") {
    gp_msg += "set terminal x11\n";
  } else {
    string f = params.get_destination_filename();
    string extension = f.substr(f.length() - 4);
    if (extension == ".eps") {
      gp_msg += "set terminal postscript color\n";
      gp_msg += "set output '" + f + "'\n";
    } else if (extension == ".png") {
      gp_msg += "set terminal png\n";
      gp_msg += "set output '" + f + "'\n";
    } else {
       cerr << "*** ERROR *** ";
       cerr << "Destination file format not supported, using X11" << endl;
       gp_msg += "set terminal x11\n";
    }
  }
  gp_msg += "unset key\n";
  gp_msg += "set xlabel '" + params.get_x_label() + "'\n";
  gp_msg += "set ylabel '" + params.get_y_label() + "'\n";
  gp_msg += "set title '" + params.get_title() + "'\n";
  if (params.get_x_min() < params.get_x_max()) {
    gp_msg += "set xrange [" + to_string(params.get_x_min()) + ":" +
              to_string(params.get_x_max()) + "]\n";
  }
  if (params.get_y_min() < params.get_y_max()) {
    gp_msg += "set yrange [" + to_string(params.get_y_min()) + ":" +
              to_string(params.get_y_max()) + "]\n";
  }
  if (params.get_x_tics()) {
    gp_msg += "set grid xtics\n";
  }
  if (params.get_y_tics()) {
    gp_msg += "set grid ytics\n";
  }
  if (params.get_xvline() != 0) {
    gp_msg += "set arrow from " + to_string(params.get_xvline()) + 
              ", graph 0 to " + to_string(params.get_xvline()) + 
              ", graph 1 nohead\n";
  }
}

namespace plot {

bool Plot2d(const std::vector<double>& x, const std::vector<double>& y,
            plot2d::Params params) {
  string gp_msg;
  gp_msg += "$data << EOD\n";
  for (int i = 0; i < x.size(); i++) {
    gp_msg += to_string(x[i]) + " " + to_string(y[i]) + "\n";
  }
  gp_msg += "EOD\n";

  set_common_parameters(gp_msg, params);

  gp_msg += "plot $data with lines lc '" + params.colors[0] + "'\n";

  Gnuplot gp;
  gp << gp_msg << endl;

  return EXIT_SUCCESS;
}

bool Plot2d(const std::vector<std::vector<double> >& x,
            const std::vector<std::vector<double> >& y, plot2d::Params params) {
  string gp_msg;
  for (int v = 0; v < x.size(); v++) {
    gp_msg += "$data" + to_string(v) + " << EOD\n";
    for (int i = 0; i < x[v].size(); i++) {
      gp_msg += to_string(x[v][i]) + " " + to_string(y[v][i]) + "\n";
    }
    gp_msg += "EOD\n";
  }

  set_common_parameters(gp_msg, params);

  for (int v = 0; v < x.size(); v++) {
    if (v == 0) {
      gp_msg +=
          "plot $data" + to_string(v) + " with lines lc '" + params.colors[v];
    } else {
      gp_msg +=
          "', $data" + to_string(v) + " with lines lc '" + params.colors[v];
    }
  }
  gp_msg += "'\n";

  Gnuplot gp;
  gp << gp_msg << endl;

  return EXIT_SUCCESS;
}

bool Plot2d(const cv::Mat_<double>& x, const cv::Mat_<double>& y,
            plot2d::Params params) {
  string gp_msg;
  for (int v = 0; v < x.rows; v++) {
    gp_msg += "$data" + to_string(v) + " << EOD\n";
    for (int i = 0; i < x.cols; i++) {
      gp_msg += to_string(x(v, i)) + " " + to_string(y(v, i)) + "\n";
    }
    gp_msg += "EOD\n";
  }

  set_common_parameters(gp_msg, params);

  for (int v = 0; v < x.rows; v++) {
    if (v == 0) {
      gp_msg +=
          "plot $data" + to_string(v) + " with lines lc '" + params.colors[v];
    } else {
      gp_msg +=
          "', $data" + to_string(v) + " with lines lc '" + params.colors[v];
    }
  }
  gp_msg += "'\n";

  Gnuplot gp;
  gp << gp_msg << endl;

  return EXIT_SUCCESS;
}

bool Plot2d(const cv::Mat_<double>& x, const cv::Mat_<int>& y,
            plot2d::Params params) {
  string gp_msg;
  for (int v = 0; v < x.rows; v++) {
    gp_msg += "$data" + to_string(v) + " << EOD\n";
    for (int i = 0; i < x.cols; i++) {
      gp_msg += to_string(x(v, i)) + " " + to_string(y(v, i)) + "\n";
    }
    gp_msg += "EOD\n";
  }

  set_common_parameters(gp_msg, params);

  for (int v = 0; v < x.rows; v++) {
    if (v == 0) {
      gp_msg +=
          "plot $data" + to_string(v) + " with lines lc '" + params.colors[v];
    } else {
      gp_msg +=
          "', $data" + to_string(v) + " with lines lc '" + params.colors[v];
    }
  }
  gp_msg += "'\n";

  Gnuplot gp;
  gp << gp_msg << endl;

  return EXIT_SUCCESS;
}

bool Plot2d(const cv::Mat_<int>& x, const cv::Mat_<double>& y,
            plot2d::Params params) {
  string gp_msg;
  for (int v = 0; v < x.rows; v++) {
    gp_msg += "$data" + to_string(v) + " << EOD\n";
    for (int i = 0; i < x.cols; i++) {
      gp_msg += to_string(x(v, i)) + " " + to_string(y(v, i)) + "\n";
    }
    gp_msg += "EOD\n";
  }

  set_common_parameters(gp_msg, params);

  for (int v = 0; v < x.rows; v++) {
    if (v == 0) {
      gp_msg +=
          "plot $data" + to_string(v) + " with lines lc '" + params.colors[v];
    } else {
      gp_msg +=
          "', $data" + to_string(v) + " with lines lc '" + params.colors[v];
    }
  }
  gp_msg += "'\n";

  Gnuplot gp;
  gp << gp_msg << endl;

  return EXIT_SUCCESS;
}

bool Plot2d(const cv::Mat_<int>& x, const cv::Mat_<int>& y,
            plot2d::Params params) {
  string gp_msg;
  for (int v = 0; v < x.rows; v++) {
    gp_msg += "$data" + to_string(v) + " << EOD\n";
    for (int i = 0; i < x.cols; i++) {
      gp_msg += to_string(x(v, i)) + " " + to_string(y(v, i)) + "\n";
    }
    gp_msg += "EOD\n";
  }

  set_common_parameters(gp_msg, params);

  for (int v = 0; v < x.rows; v++) {
    if (v == 0) {
      gp_msg +=
          "plot $data" + to_string(v) + " with lines lc '" + params.colors[v];
    } else {
      gp_msg +=
          "', $data" + to_string(v) + " with lines lc '" + params.colors[v];
    }
  }
  gp_msg += "'\n";

  Gnuplot gp;
  gp << gp_msg << endl;

  return EXIT_SUCCESS;
}
}
