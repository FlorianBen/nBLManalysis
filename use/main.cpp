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

  // ScanFolder scan(argv[1], argv[2], argv[3]);
  // RawConvertorNTpule convertor(scan.GetIndex());

  // convertor.SetName(argv[2]);
  // convertor.SetFileNameOuput("output.root");
  // convertor.Convert();

  RunTigger run;
  run.LoadRNTuple(argv[1]);
  run.CreateEvents();

  // auto model = ROOT::RNTupleModel::Create();
  // auto signals = model->MakeField<DSignal>("signal");
  // auto reader =
  //     ROOT::RNTupleReader::Open(std::move(model), "DSignal",
  //     "output.root");
  // reader->PrintInfo();

  // auto sig = reader->GetView<std::vector<double>>("DSignal.signal");

  // TApplication app("App", &argc, argv);
  // TCanvas *c2 = new TCanvas("c2", "Signal", 200, 10, 700, 500);
  // TH1F h("h", "Signal", 512, -1, 7);

  // // reader->Show(41);

  // for (auto i : reader->GetEntryRange()) {
  //   reader->LoadEntry(i);
  // }

  // h.Draw();

  // app.Run();

  return 0;
}
