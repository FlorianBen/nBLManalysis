#include "nBLM/DSignal.hpp"
#include "nBLM/IOLecroy.hpp"
#include "nBLM/RawConvertor.hpp"
#include "nBLM/Run.hpp"
#include "nBLM/ScanFolder.hpp"

#include "ROOT/RNTupleModel.hxx"
#include "ROOT/RNTupleReader.hxx"
#include "ROOT/RNTupleWriter.hxx"
#include <ROOT/RDataFrame.hxx>

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TRandom.h"
#include <TH1F.h>

int main(int argc, char *argv[]) {
  auto model = ROOT::RNTupleModel::Create();
  auto reader = ROOT::RNTupleReader::Open(std::move(model), "Events", argv[1]);
  reader->PrintInfo();
  auto data = reader->GetView<std::vector<double>>("Event.data");
  auto dT = 2e-9;
  auto ToT = reader->GetView<double>("Event.ToT");
  auto id = reader->GetView<int>("Event.id_signal");

  auto vec = data(0);
  for (auto entry : *reader) {
    if (ToT(entry) > 2000e-9) {
      std::cout << "Found pulse " << id(entry) << std::endl;
      vec = data(entry);
      break;
    }
  }

  std::vector<double> time_i(vec.size());
  std::generate(std::begin(time_i), std::end(time_i),
                [n = 0, &dT]() mutable { return dT * n++; });

  TApplication app("InspectSignal", &argc, argv);
  TCanvas *c2 = new TCanvas("c1", "Signal", 200, 10, 700, 500);
  c2->SetGrid();
  auto gr_signal = new TGraph(vec.size(), &time_i[0], &vec[0]);
  gr_signal->Draw();
  c2->SaveAs("pileup.png");
  app.Run();

  return 0;
}