#pragma once

#include <string>
#include <vector>

class ScanFolder {
private:
  std::string folder;
  std::string channel;
  std::string prefix;
  std::vector<std::pair<int, std::string>> indexs;

  void DoScan();

  void SortIndex();

public:
  ScanFolder(std::string folder, std::string channel, std::string prefix);

  std::vector<std::pair<int, std::string>> GetIndex();
};
