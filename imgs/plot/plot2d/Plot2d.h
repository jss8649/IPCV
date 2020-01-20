/** Interface file for two-dimensional plotting
 *
 *  \file plot/plot2d/Plot2d.h
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \date 28 Aug 2018
 */

#pragma once

#include <vector>
#include <opencv2/core.hpp>

#include "imgs/plot/plot2d/Params.h"

namespace plot {

/** Plot two-dimensional data
 *
 *  \param[in] x independent variable vector of type double
 *  \param[in] y dependent variable vector of type double
 *  \param[in] p plot parameters object of type plot2d::Params
 *
 *  \return a boolean indicating that the plotting operation has been
 *          carried out without error
 */
bool Plot2d(const std::vector<double>& x, const std::vector<double>& y,
            plot2d::Params params);

/** Plot two-dimensional data (multiple datasets)
 *
 *  \param[in] x independent variable vector of vectors of type double
 *  \param[in] y dependent variable vector of vectors of type double
 *  \param[in] p plot parameters object of type plot2d::Params
 *
 *  \return a boolean indicating that the plotting operation has been
 *          carried out without error
 */
bool Plot2d(const std::vector<std::vector<double> >& x,
            const std::vector<std::vector<double> >& y, plot2d::Params params);

/** Plot two-dimensional data (single/multiple datasets)
 *
 *  \param[in] x independent variable cv::Mat_ with 1 or multiple rows
 *  \param[in] y dependent variable cv::Mat_ with 1 or multiple rows
 *  \param[in] p plot parameters object of type plot2d::Params
 *
 *  \return a boolean indicating that the plotting operation has been
 *          carried out without error
 */
bool Plot2d(const cv::Mat_<double>& x, const cv::Mat_<double>& y, 
            plot2d::Params params);
bool Plot2d(const cv::Mat_<int>& x, const cv::Mat_<double>& y, 
            plot2d::Params params);
bool Plot2d(const cv::Mat_<double>& x, const cv::Mat_<int>& y, 
            plot2d::Params params);
bool Plot2d(const cv::Mat_<int>& x, const cv::Mat_<int>& y, 
            plot2d::Params params);
}
