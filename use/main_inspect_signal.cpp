#include "nBLM/DSignal.hpp"
#include "nBLM/IOLecroy.hpp"
#include "nBLM/RawConvertor.hpp"
#include "nBLM/Run.hpp"
#include "nBLM/ScanFolder.hpp"

#include "Compression.h"
#include "ROOT/RNTupleModel.hxx"
#include "ROOT/RNTupleReader.hxx"
#include "ROOT/RNTupleWriter.hxx"

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TRandom.h"
#include <TH1F.h>

int main(int argc, char *argv[]) {
  auto model = ROOT::RNTupleModel::Create();
  auto reader = ROOT::RNTupleReader::Open(std::move(model), "DSignal", argv[1]);
  reader->PrintInfo();
  auto sig = reader->GetView<std::vector<double>>("DSignal.signal");
  auto dT = reader->GetView<double>("DSignal.dT")(0);

  auto i = 0;
  if (argc == 3) {
    i = std::atoi(argv[2]);
  }

  auto vec = sig(i);

  std::vector<double> time_i(vec.size());
  std::generate(std::begin(time_i), std::end(time_i),
                [n = 0, &dT]() mutable { return dT * 1000 * n++; });

  TApplication app("InspectSignal", &argc, argv);
  TCanvas *c2 = new TCanvas("c1", "Signal", 200, 10, 700, 500);
  auto gr_signal = new TGraph(vec.size(), &time_i[0], &vec[0]);
  gr_signal->Draw();
  app.Run();
  return 0;
}