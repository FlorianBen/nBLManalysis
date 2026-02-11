#include "nBLM/ScanFolder.hpp"

#include <filesystem>
#include <iostream>
#include <regex>

ScanFolder::ScanFolder(std::string folder, std::string channel,
                       std::string prefix)
    : folder(folder), channel(channel), prefix(prefix) {
  DoScan();
  SortIndex();
}

void ScanFolder::DoScan() {
  auto num_of_file = 0;
  std::smatch match;
  const std::string reg_str(channel);
  std::regex reg(reg_str);

  const std::string reg_str_index(prefix + "(\\d{3,6})");
  std::regex reg_index(reg_str_index);

  for (const auto &entry : std::filesystem::directory_iterator(folder)) {
    if (entry.path().extension() ==
        std::filesystem::path("foo.trc").extension()) {
      const auto filename = entry.path().filename().string();
      if (std::regex_search(filename, match, reg)) {
        if (std::regex_search(filename, match, reg_index)) {
          const auto index_tmp = std::atoi(match[1].str().c_str());
          indexs.push_back({index_tmp, entry.path().string()});
        }
        num_of_file++;
      }
    }
  }
}

void ScanFolder::SortIndex() {
  std::stable_sort(
      indexs.begin(), indexs.end(),
      [](const auto &a, const auto &b) { return a.first < b.first; });
}

std::vector<std::pair<int, std::string>> ScanFolder::GetIndex() {
  return indexs;
}