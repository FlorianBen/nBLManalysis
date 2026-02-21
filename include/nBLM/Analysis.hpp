#pragma once

#include "ROOT/RNTupleModel.hxx"
#include "ROOT/RNTupleReader.hxx"
#include "ROOT/RNTupleView.hxx"

#include "TH1D.h"
#include "TH2D.h"

#include <utility>

#include "nBLM/DSignal.hpp"
#include "nBLM/Event.hpp"

class Analysis {
  using unique_hist1D = std::unique_ptr<TH1D>;
  using hist1D_pair = std::pair<unique_hist1D, unique_hist1D>;
  using unique_hist2D = std::unique_ptr<TH2D>;

public:
  hist1D_pair hist1D_amplitude;
  hist1D_pair hist1D_integral;
  hist1D_pair hist1D_ToT;
  hist1D_pair hist1D_Tr;
  hist1D_pair hist1D_Td;
  hist1D_pair hist1D_events;
  hist1D_pair hist1D_timing;

  unique_hist2D hist2D_amplitude_time;
  unique_hist2D hist2D_integrale_time;
  unique_hist2D hist2D_ToT_time;

  std::unique_ptr<ROOT::RNTupleModel> model_event;
  std::unique_ptr<ROOT::RNTupleReader> reader_event;

  std::unique_ptr<ROOT::RNTupleModel> model_signal;
  std::unique_ptr<ROOT::RNTupleReader> reader_signal;

  Analysis(std::string filename_signal, std::string datasetname_signal,
           std::string filename_event, std::string datasetname_event);

  void InitHistogram1D();
  void InitHistogram2D();

  void Process();
};