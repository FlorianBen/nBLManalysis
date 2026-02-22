#include "nBLM/Processing.hpp"

#include <algorithm>
#include <numeric>

Smooth::Smooth(const int order, const double dt) : order(order), dt(dt) {}

std::vector<double>
Smooth::operator()(const std::vector<double> &signal) const {
  auto size = signal.size();
  std::vector<double> out(size);
  auto np = order;
  if (np <= 0)
    np = 1;
  np = (np / 2) * 2 + 1;

  for (int i = 0; i < np / 2 - 1; i++)
    out[i] = signal[i];
  auto sum =
      std::accumulate(std::begin(signal), std::begin(signal) + np - 1, 0.0);
  out[np / 2] = sum / np;

  for (int i = np / 2 + 1; i < size - np / 2; i++) {
    sum -= signal[i - (np / 2 + 1)];
    sum += signal[i + np / 2];
    out[i] = sum / np;
  }
  for (int i = np / 2 + 1 - 1; i >= 0; i--) {
    out[i] = out[i + 1];
  }
  for (int i = size - np / 2; i < size; i++) {
    out[i] = out[i - 1];
  }
  return std::move(out);
}

BaselineSupress::BaselineSupress(const double baseline) : baseline(baseline) {}

std::vector<double>
BaselineSupress::operator()(const std::vector<double> &signal) const {
  auto size = signal.size();
  std::vector<double> new_v(size);
  std::transform(std::begin(signal), std::end(signal), std::begin(new_v),
                 [this](auto val) { return val - baseline; });
  return std::move(new_v);
}

BaselineCompute::BaselineCompute(const int points, typeSide side)
    : points(points), side(side) {
  compute = typeCompute::EDGE;
}

BaselineCompute::BaselineCompute(const int points, typeCompute compute)
    : points(points), compute(compute) {}

double BaselineCompute::operator()(const std::vector<double> &signal) const {
  auto size = signal.size();

  switch (compute) {
  case typeCompute::EDGE:
    if (side == typeSide::LEFT) {
      return std::accumulate(signal.begin(), signal.begin() + points, .0) /
             points;
    } else {
      return std::accumulate(signal.end() - points, signal.end(), .0) / points;
    }
    break;

  case typeCompute::BEDGES:
    break;

  case typeCompute::RANGE:
    break;

  default:
    break;
  }

  return std::accumulate(signal.end() - size, signal.end(), .0) / size;
}

// CutHistogram::CutHistogram(const double cut) : cut(cut) {}

// int CutHistogram::operator()(TH1 *hist) const {
//   auto scale = fabs(cut);
//   if (scale > 1)
//     return 0;
//   auto sx = hist->GetMaximum();

//   int n1 = hist->GetNbinsX();
//   for (int i = 0; i <= n1; i++) {
//     auto x = hist->GetBinContent(i);
//     if (x < scale * sx) {
//       hist->SetBinContent(i, 0.);
//     }
//   }
//   return 1;
// }