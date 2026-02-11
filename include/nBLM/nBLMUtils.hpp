#pragma once

#include <cmath>
#include <string>

/**
 * @brief PI definition
 *
 */
constexpr double pi = M_PI;

/**
 * @brief Converts an int into string and pads 0 char.
 *
 * @param number Number to convert.
 * @param padding Padding size.
 * @return std::string Output string
 */
std::string zfill(const int number, const int padding);

/**
 * @brief Moyal function (more generic Landau distribution)
 *
 * @param x Evaluation point.
 * @param par Parameters array: [0] amplitude, [1] MPV, [2] sigma.
 * @return double Function value.
 */
double moyal_fn(const double *x, const double *par);

/**
 * @brief Exponential function.
 *
 * @param x Evaluation point.
 * @param par Parameters array: [0] amplitude, [1] tau, [2] constant.
 * @return double Function value.
 */
double exp_fn(const double *x, const double *par);

/**
 * @brief Double exponential function.
 *
 * @param x Evaluation point.
 * @param par Parameters array: [0] amplitude 1, [1] tau 1, [2] amplitude 1, [3]
 * tau 1, [4] constant.
 * @return double Function value.
 */
double exp_d_fn(const double *x, const double *par);
