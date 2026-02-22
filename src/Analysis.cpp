#include "nBLM/Analysis.hpp"

Analysis::Analysis(std::string filename_signal, std::string datasetname_signal,
                   std::string filename_event, std::string datasetname_event) {
  model_signal = ROOT::RNTupleModel::Create();
  reader_signal = ROOT::RNTupleReader::Open(
      std::move(model_signal), datasetname_signal, filename_signal);

  model_event = ROOT::RNTupleModel::Create();
  reader_event = ROOT::RNTupleReader::Open(std::move(model_event),
                                           datasetname_event, filename_event);

  InitHistogram1D();
  InitHistogram2D();
}

void Analysis::InitHistogram1D() {
  auto signal = reader_signal->GetView<DSignal>("DSignal");
  auto event = reader_event->GetView<Event>("Event");

  hist1D_amplitude = std::make_pair(
      std::make_unique<TH1D>("hist1D_amplitude", "Histogram Amplitude Event",
                             256, 0.0, 1.6),
      std::make_unique<TH1D>("hist1D_amplitude_neutron",
                             "Histogram Amplitude Neutron", 256, 0.0, 1.6));
  hist1D_integral = std::make_pair(
      std::make_unique<TH1D>("hist1D_integral", "Histogram Integral Event", 256,
                             0.0, 127),
      std::make_unique<TH1D>("hist1D_integral_neutron",
                             "Histogram Integral Neutron", 256, 0.0, 127));
  hist1D_ToT = std::make_pair(
      std::make_unique<TH1D>("hist1D_ToT", "Histogram ToT Event", 256, 0.0,
                             1023e-9),
      std::make_unique<TH1D>("hist1D_ToT_neutron", "Histogram ToT Neutron", 256,
                             0.0, 1023e-9));
  hist1D_Tr = std::make_pair(
      std::make_unique<TH1D>("hist1D_Tr", "Histogram Tr Event", 128, 0.0,
                             255e-9),
      std::make_unique<TH1D>("hist1D_Tr_neutron", "Histogram Tr Neutron", 128,
                             0.0, 255e-9));
}

void Analysis::InitHistogram2D() {
  auto signal = reader_signal->GetView<DSignal>("DSignal");
  auto fsignal = signal(0);
  auto lsignal = signal(reader_signal->GetEntryRange().size() - 1);

  auto event = reader_event->GetView<Event>("Event");

  hist2D_amplitude_time = std::make_unique<TH2D>(
      "hist2D_amplitude_time", "Histogram Amplitude over Time", 256, 0.0,
      fsignal.signal.size() * fsignal.dT, 256, 0.0, 1.6);

  hist2D_integrale_time = std::make_unique<TH2D>(
      "hist2D_integrale_time", "Histogram Integrale over Time", 256, 0.0,
      fsignal.signal.size() * fsignal.dT, 256, 0.0, 127);

  hist2D_ToT_time = std::make_unique<TH2D>(
      "hist2D_ToT_time", "Histogram ToT over Time", 256, 0.0,
      fsignal.signal.size() * fsignal.dT, 256, 0.0, 1023e-9);
}

void Analysis::Process() {
  auto signal = reader_signal->GetView<DSignal>("DSignal");
  auto fsignal = signal(0);

  auto event = reader_event->GetView<Event>("Event");

  for (auto i : reader_event->GetEntryRange()) {
    auto ev = event(i);
    hist1D_amplitude.first->Fill(std::abs(ev.amplitude));
    hist1D_integral.first->Fill(std::abs(ev.integral));
    hist1D_ToT.first->Fill(std::abs(ev.ToT));
    hist2D_amplitude_time->Fill(ev.idx_max * fsignal.dT,
                                std::abs(ev.amplitude));
    hist2D_integrale_time->Fill(ev.idx_max * fsignal.dT, std::abs(ev.integral));
    hist2D_ToT_time->Fill(ev.idx_max * fsignal.dT, std::abs(ev.ToT));

    hist1D_Tr.first->Fill(std::abs(ev.Tr));
  }
}