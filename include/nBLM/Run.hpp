#pragma once

#include <memory>
#include <string>

#include "ROOT/RNTupleModel.hxx"
#include "ROOT/RNTupleReader.hxx"

class RunTigger {
private:
  /* data */
  std::unique_ptr<ROOT::RNTupleModel> model;
  std::unique_ptr<ROOT::RNTupleReader> reader;

public:
  RunTigger(/* args */);

  void LoadRNTuple(std::string filename, std::string datasetname = "DSignal");
  void CreateEvents();
};