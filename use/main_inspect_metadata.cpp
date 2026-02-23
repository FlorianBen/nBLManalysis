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

  auto hints = ScanFolder::HintChannelFolder(argv[1]);
  for (auto hint : hints) {
    std::cout << hint << std::endl;
  }

  ScanFolder scan(argv[1], argv[2], argv[3]);
  scan.PrintInfo();

  RawConvertorNTpule convertor(scan.GetIndex());

  // convertor.PrintMedatadata(0);
  convertor.PrintDateRange();

  return 0;
}
