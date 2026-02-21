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

  ScanFolder scan(argv[1], argv[2], argv[3]);
  RawConvertorNTpule convertor(scan.GetIndex());

  convertor.SetFileNameOuput(argv[4]);
  convertor.Convert();

  return 0;
}
