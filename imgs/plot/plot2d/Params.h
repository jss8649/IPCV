/** Interface file for plot parameter specification
 *
 *  \file plot/plot2d/Params.h
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 23 Mar 2018
 */

#pragma once

#include <string>

namespace plot::plot2d {

class Params {
 public:
  /** Constructors for the plot parameters object
   *
   *  \param[in] x_label a string containing the x-axis label
   *  \param[in] y_label a string containing the y-axis label
   */
  Params();

  Params(std::string x_label, std::string y_label);

  /** Destructor for the plot parameters object
   */
  ~Params();

  /** Accessor and mutator for the x_label member
   */
  std::string get_x_label();
  void set_x_label(std::string x_label);

  /** Accessor and mutator for the y_label member
   */
  std::string get_y_label();
  void set_y_label(std::string y_label);

  /** Accessor and mutator for the title member
   */
  std::string get_title();
  void set_title(std::string title);

  /** Accessor and mutator for the x_min and x_max members
   */
  double get_x_min();
  void set_x_min(double x_min);
  double get_x_max();
  void set_x_max(double x_max);

  /** Accessor and mutator for the y_min and y_max members
   */
  double get_y_min();
  void set_y_min(double y_min);
  double get_y_max();
  void set_y_max(double y_max);

  /** Accessor and mutator for the x_tics and y_tics members
   */
  bool get_x_tics();
  void set_x_tics(bool x_tics);
  bool get_y_tics();
  void set_y_tics(bool y_tics);

  /** Accessor and mutator for the xvline member
   */
  double get_xvline();
  void set_xvline(double xvline);

  /** Accessor and mutator for the destination filename member
   */
  std::string get_destination_filename();
  void set_destination_filename(std::string dst_filename);

  /** Print function to carry out a diagnostic dump of object values
   */
  void Print();

  /** Class public constants
   */
  const std::string colors[6] = {
    "blue",
    "green",
    "red",
    "cyan",
    "magenta",
    "yellow"
  };

 private:
  std::string x_label_ = "";
  std::string y_label_ = "";
  std::string title_ = "";
  double x_min_ = 0;
  double x_max_ = -1;
  double y_min_ = 0;
  double y_max_ = -1;
  bool x_tics_ = false;
  bool y_tics_ = false;
  double xvline_ = 0;
  std::string dst_filename_ = "";
};
}
