#include "nBLM/Run.hpp"

#include "ROOT/RNTupleWriter.hxx"
#include "nBLM/Trigger.hpp"

RunTigger::RunTigger() {}

void RunTigger::LoadRNTuple(std::string filename, std::string datasetname) {
  model = ROOT::RNTupleModel::Create();
  reader = ROOT::RNTupleReader::Open(std::move(model), datasetname, filename);
}

void RunTigger::CreateEvents() {
  auto model_out = ROOT::RNTupleModel::Create();
  auto event_f = model_out->MakeField<Event>("Event");
  ROOT::RNTupleWriteOptions options;
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model_out), "Events",
                                              "output.root", options);

  auto sig = reader->GetView<std::vector<double>>("DSignal.signal");
  auto t0 = reader->GetView<TimeStamp>("DSignal.t0");
  auto dT = reader->GetView<double>("DSignal.dT")(0);

  Trigger trig(eTrigger::FALLING, -0.03, 5);
  for (auto i : reader->GetEntryRange()) {
    // reader->LoadEntry(i);
    auto vec = sig(i);
    auto ts0 = t0(i);
    std::cout << i << " -- " << trig(vec) << std::endl;
    for (auto idx : trig.indexes) {
      // Event ev;
      event_f->indexes = idx;
      event_f->ProcessEvent(vec, dT);

      TimeStamp pulse_ts(ts0);
      pulse_ts.frac_sec = ts0.frac_sec + event_f->idx_max * dT;

      writer->Fill();
    }
  }
}