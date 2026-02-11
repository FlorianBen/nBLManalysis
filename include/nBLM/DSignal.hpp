#pragma once

#include <vector>

#include "nBLM/Event.hpp"
#include "nBLM/TimeStamp.hpp"

class DSignal {
public:
  std::vector<double> signal;
  TimeStamp t0;
  double offset;
  double dT;
  int index;

public:
};
