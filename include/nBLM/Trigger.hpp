#pragma once

#include <vector>

#include "nBLM/Event.hpp"

enum eTrigger { RISING = 1, FALLING = -1 };

class Trigger {
  using indexv = std::vector<std::pair<int, int>>;

private:
  /* data */
  eTrigger trig_pol;
  double threshold;
  int multi;

  indexv trigger_sig(const std::vector<double> &signal, const double threshold,
                     const int multiplicity);

  bool check_pol(const std::vector<double> &data, const int index,
                 const int backmax);

public:
  indexv indexes;
  std::vector<Event> events;

public:
  Trigger(eTrigger trigger_pol, double threshold = -0.02, int multiplicity = 3);

  const eTrigger trigger_pol() const;
  void trigger_pol(const eTrigger trigger_pol);

  const int multiplicity() const;
  void multiplicity(const int multiplicity);

  std::size_t operator()(const std::vector<double> &signal);
  std::size_t operator()(const std::vector<double> &signal, double threshold);
  std::size_t operator()(const std::vector<double> &signal, double threshold,
                         int multiplicity);
};