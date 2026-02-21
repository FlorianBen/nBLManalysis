#include "nBLM/Analysis.hpp"

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

  Analysis analysis(argv[1], argv[2], argv[3], argv[4]);
  analysis.Process();

  TApplication app("Analysis", &argc, argv);
  TCanvas *c2 = new TCanvas("c1", "Histogram", 200, 10, 700, 500);
  c2->Divide(3, 3);
  c2->cd(1);
  analysis.hist1D_amplitude.first->Draw();
  c2->cd(2);
  analysis.hist1D_integral.first->Draw();
  c2->cd(3);
  analysis.hist1D_ToT.first->Draw();

  c2->cd(4);
  analysis.hist2D_amplitude_time->Draw();
  c2->cd(5);
  analysis.hist2D_integrale_time->Draw();
  c2->cd(6);
  analysis.hist2D_ToT_time->Draw();

  app.Run();
  return 0;
}
