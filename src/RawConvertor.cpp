#include "nBLM/RawConvertor.hpp"

#include "ROOT/RNTupleModel.hxx"
#include "ROOT/RNTupleReader.hxx"
#include "ROOT/RNTupleWriter.hxx"

#include "nBLM/DSignal.hpp"
#include "nBLM/IOLecroy.hpp"
#include "nBLM/Processing.hpp"
#include "nBLM/TimeStamp.hpp"

RawConvertorNTpule::RawConvertorNTpule(
    std::vector<std::pair<int, std::string>> filenames_in)
    : filenames_in(filenames_in), name("DSignal"), filename_out("out.root"),
      folder_out("") {}

void RawConvertorNTpule::Convert() {

  auto model = ROOT::RNTupleModel::Create();
  auto dsignal = model->MakeField<DSignal>("DSignal");
  ROOT::RNTupleWriteOptions options;
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), name,
                                              filename_out, options);
  auto index = 0;

  for (auto file : filenames_in) {
    LecroyReader lecroyreader(file.second, true);
    const auto metadata = lecroyreader.GetMetadata();
    const auto full_data = lecroyreader.GetSignal();
    const auto trigtimes = lecroyreader.GetTrigtimes();
    const auto array_size = metadata.WAVE_ARRAY_COUNT / metadata.SUBARRAY_COUNT;
    for (auto i = 0; i < metadata.SUBARRAY_COUNT; i++) {
      std::vector<double> signal_temp(
          std::make_move_iterator(std::begin(full_data)) + i * array_size,
          std::make_move_iterator(std::begin(full_data)) +
              (i + 1) * array_size);
      BaselineCompute bc(0.05 * array_size, BaselineCompute::typeSide::LEFT);
      auto offset = bc(signal_temp);
      BaselineSupress bs(offset);
      signal_temp = bs(signal_temp);
      Smooth sm(3, metadata.HORIZ_INTERVAL);
      signal_temp = sm(signal_temp);
      dsignal->signal = signal_temp;
      dsignal->t0 = TimeStamp(metadata.TRIGGER_TIME, trigtimes[i]);
      dsignal->dT = metadata.HORIZ_INTERVAL;
      dsignal->offset = metadata.ACQ_VERT_OFFSET - offset;
      dsignal->offset = index++;
      writer->Fill();
    }
  }
}

void RawConvertorNTpule::SetName(std::string name) { this->name = name; }

void RawConvertorNTpule::SetFileNameOuput(std::string filename_out) {
  this->filename_out = filename_out;
}

void RawConvertorNTpule::SetFolderOutput(std::string folder_out) {
  this->name = folder_out;
}

void RawConvertorNTpule::SortIndex() {
  std::sort(filenames_in.begin(), filenames_in.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });
}

void RawConvertorNTpule::SortMedatadata() {
  std::sort(
      std::begin(filenames_in), std::end(filenames_in),
      [this](const auto ele1, const auto ele2) -> bool {
        LecroyReader reader1(ele1.second, false, false);
        LecroyReader reader2(ele2.second, false, false);
        TimeStamp ts1(reader1.GetMetadata().TRIGGER_TIME, Trigtime{0.0, 0.0});
        TimeStamp ts2(reader2.GetMetadata().TRIGGER_TIME, Trigtime{0.0, 0.0});
        return ts1 < ts2;
      });
}