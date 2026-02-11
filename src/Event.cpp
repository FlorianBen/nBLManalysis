#include "nBLM/Event.hpp"

#include <algorithm>
#include <numeric>

void Event::ProcessEvent(const std::vector<double> &signal, const double dT) {
  auto win_pre = 50;
  auto win_post = 50;

  const auto [max, min] =
      std::minmax_element(std::begin(signal) + indexes.first - win_pre,
                          std::begin(signal) + indexes.second + win_post);

  data = std::vector<double>(signal.begin() + indexes.first - win_pre,
                             signal.begin() + indexes.second + win_post);

  auto DR = *max - *min;
  auto amp_index_max = std::distance(std::begin(signal), max);
  auto amp_index_min = std::distance(std::begin(signal), min);

  integral = std::accumulate(std::begin(signal) + indexes.first,
                             std::begin(signal) + indexes.second, 0.0);

  auto level90 = DR * 0.9 + *min;
  auto level10 = DR * 0.1 + *min;
  auto tamp90_2 = 0;
  auto tamp10_2 = 0;
  for (auto it = max; it != std::begin(signal); it--) {
    if (*it > level90) {
      tamp90_2 = std::distance(std::begin(signal), it);
      break;
    }
  }
  for (auto it = max; it != std::begin(signal); it--) {
    if (*it > level10) {
      tamp10_2 = std::distance(std::begin(signal), it);
      break;
    }
  }

  auto count_sat = std::count_if(std::begin(signal) + indexes.first,
                                 std::begin(signal) + indexes.second,
                                 [max](auto ele) { return ele <= *max; });
  //   if (count_sat > 6) {
  //     pulse.saturated = true;
  //   }

  auto amp90 =
      std::upper_bound(std::rbegin(signal) + (signal.size() - indexes.second),
                       std::rend(signal), DR * 0.9 + *min);
  auto tamp90 = std::distance(std::rend(signal), amp90);

  auto amp10 =
      std::upper_bound(std::rbegin(signal) + (signal.size() - indexes.second),
                       std::rend(signal), DR * 0.1 + *min);
  auto tamp10 = std::distance(std::rend(signal), amp10);

  this->max = *min;
  this->max = *max;
  amplitude = DR;
  idx_max = amp_index_max;
  idx_min = amp_index_min;

  idx_ToT = indexes.second - indexes.first;
  ToT = idx_ToT * dT;
  Tr = (tamp90_2 - tamp10_2) * dT;
}