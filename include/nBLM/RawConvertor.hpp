#pragma once

#include <string>
#include <vector>

class RawConvertorNTpule {
private:
  std::vector<std::pair<int, std::string>> filenames_in;
  std::string name;
  std::string filename_out;
  std::string folder_out;
  bool preprocess;

public:
  RawConvertorNTpule();
  RawConvertorNTpule(std::string filename_in);
  RawConvertorNTpule(std::vector<std::pair<int, std::string>> filenames_in);

  void AddFile(std::string filename_in);
  void AddFiles();

  void Convert();

  void SetName(std::string name);

  void SetFileNameOuput(std::string filename_out);
  void SetFolderOutput(std::string folder_out);

  void SortIndex();
  void SortMedatadata();
};