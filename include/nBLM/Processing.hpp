#pragma once

#include <queue>
#include <vector>

/**
 * @brief (WIP) Processing is the base class for all the processing classes used
 * for the nBLM. Processing classes apply or compute from an input signal.
 */
class Processing {
private:
  /* data */
public:
  Processing(/* args */);
  ~Processing();

  virtual std::vector<double> operator()(std::vector<double> &signal) const;
  virtual double operator()(const std::vector<double> &signal) const;
};

class ProcessingList {
public:
  std::queue<Processing> processing_list;

  void AddProcessing(const Processing &processing);

  void Process();
};

/**
 * @brief (WIP) Derivate the signal according to a ti;e constant with a certain
 * order. It can be useful for finding minima and maxima of a signal.
 */
class Derivate {
private:
  double dt;
  int order;

public:
  Derivate() noexcept = default;
  ~Derivate() noexcept = default;

  std::vector<double> operator()(std::vector<double> &signal) const;
};

/**
 * @brief Basic high frequencies filtering of a signal.
 */
class Smooth {
private:
  double dt;
  int order;

public:
  Smooth(const int order = 3, const double dt = 4e-9);
  ~Smooth() noexcept = default;

  std::vector<double> operator()(const std::vector<double> &signal) const;
};

/**
 * @brief Compute the baseline of signal by different method.
 */
class BaselineCompute {
public:
  /// Type of computation.
  enum class typeCompute { EDGE, BEDGES, RANGE };
  /// Side
  enum class typeSide { LEFT, RIGHT, BOTH };

private:
  typeCompute compute;
  typeSide side;
  int points;
  int indx_a, indx_b;

public:
  BaselineCompute(const int points, typeSide side = typeSide::RIGHT);
  BaselineCompute(const int points, typeCompute compute);
  BaselineCompute(const int indx_a, const int indx_b, typeCompute compute);
  ~BaselineCompute() noexcept = default;

  double operator()(const std::vector<double> &signal) const;
};

/**
 * @brief Remove the baseline or any defined value from the signal.
 */
class BaselineSupress {
private:
  double baseline;

public:
  BaselineSupress(const double baseline = 0.0);
  ~BaselineSupress() noexcept = default;

  std::vector<double> operator()(const std::vector<double> &signal) const;
  // void operator()(std::vector<double> &signal) const;
};

/**
 * @brief (NOT IMPLEMENTED) Compute the baseline by using histogram.
 */
class BaselineHistogram {
private:
public:
  /**
   *
   */
  BaselineHistogram();
  ~BaselineHistogram() noexcept = default;

  std::vector<double> operator()(const std::vector<double> &signal) const;
  void operator()(std::vector<double> &signal) const;
};

class Integrate {
private:
  double dt;
  int order;

public:
  Integrate() noexcept = default;
  ~Integrate() noexcept = default;

  std::vector<double> operator()(std::vector<double> &signal) const;
};
