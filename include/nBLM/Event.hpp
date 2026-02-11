#pragma once

#include <utility>
#include <vector>

#include "nBLM/TimeStamp.hpp"

class Event {
public:
  std::pair<int, int> indexes;
  std::vector<double> data;
  TimeStamp t0;

  double amplitude;
  double integral;
  double min;
  double max;
  int idx_min;
  int idx_max;
  double Tr;
  double Td;
  double ToT;
  int idx_ToT;

  int id_signal;

public:
  void ProcessEvent(const std::vector<double> &signal, const double dT = 2e-9);
};
