#include "nBLM/nBLMUtils.hpp"

std::string zfill(const int number, const int padding) {
  std::string old_str = std::to_string(number);
  return std::string(padding -
                         std::min(padding, static_cast<int>(old_str.length())),
                     '0') +
         old_str;
}

double moyal_fn(const double *x, const double *par) {
  auto lambda = (x[0] - par[1]) / par[2];
  auto moyal = par[0] * (std::exp(-0.5 * (lambda + std::exp(-lambda)))) /
               (std::sqrt(2 * pi));
  return moyal;
}

double exp_fn(const double *x, const double *par) {
  return par[0] * exp(-par[1] * x[0]) + par[2];
}

double exp_d_fn(const double *x, const double *par) {
  return par[0] * exp(-par[1] * x[0]) + par[2] * exp(-par[3] * x[0]) + par[4];
}
