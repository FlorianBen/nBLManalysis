#include "nBLM/Trigger.hpp"

#include <algorithm>
#include <numeric>

Trigger::Trigger(eTrigger pol, double threshold, int multiplicity)
    : trig_pol(pol), threshold(threshold), multi(multiplicity) {}

const eTrigger Trigger::trigger_pol() const { return trig_pol; }

void Trigger::trigger_pol(const eTrigger trigger_pol) {
  trig_pol = trigger_pol;
}

const int Trigger::multiplicity() const { return multi; }

void Trigger::multiplicity(const int multiplicity) { multi = multiplicity; }

std::size_t Trigger::operator()(const std::vector<double> &signal) {
  indexes = trigger_sig(signal, threshold, multi);
  return indexes.size();
}

std::size_t Trigger::operator()(const std::vector<double> &signal,
                                double threshold) {
  indexes = trigger_sig(signal, threshold, multi);
  return indexes.size();
}

std::size_t Trigger::operator()(const std::vector<double> &signal,
                                double threshold, int multiplicity) {
  indexes = trigger_sig(signal, threshold, multiplicity);
  return indexes.size();
}

Trigger::indexv Trigger::trigger_sig(const std::vector<double> &signal,
                                     const double threshold,
                                     const int multiplicity) {
  indexv indexes;
  auto size = signal.size();
  auto data = signal;

  events.clear();

  std::size_t trigi_ind = 0;
  std::size_t trigf_ind = 0;

  auto count_multi = 0;
  for (auto i = 0; i < size; i++) {
    if ((trig_pol * data[i]) > (trig_pol * threshold)) {
      count_multi++;
      if ((count_multi > multiplicity) && (check_pol(data, i, 10))) {
        trigf_ind = 0;
        trigi_ind = i - multiplicity;
        count_multi = 0;
        for (auto j = i; j < size; j++) {
          i = j;

          if ((trig_pol * data[j]) < (trig_pol * threshold)) {
            count_multi++;
            if (count_multi > multiplicity) {
              trigf_ind = j - multiplicity;
              break;
            }
          }
        }
        if (trigf_ind > trigi_ind) {
          indexes.push_back(std::make_pair(trigi_ind, trigf_ind));
        } else {
          // TODO: Throw something here...
        }
      }
    } else {
      count_multi = 0;
    }
  }
  return std::move(indexes);
}

bool Trigger::check_pol(const std::vector<double> &data, const int index,
                        const int backmax) {
  std::vector<double> derivative;

  std::adjacent_difference(data.begin() + index - backmax, data.begin() + index,
                           std::back_inserter(derivative));
  derivative.erase(derivative.begin());

  size_t count_negative = std::count_if(derivative.begin(), derivative.end(),
                                        [](double x) { return x < 0; });

  return (count_negative > multi);
}