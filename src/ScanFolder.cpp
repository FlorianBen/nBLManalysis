#include "nBLM/ScanFolder.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <regex>
#include <unordered_set>

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

void ScanFolder::PrintInfo() {
  std::cout << "File found: " << indexs.size() << std::endl;
}

std::vector<std::string>
ScanFolder::HintChannelFolder(const std::string folder) {
  std::vector<std::string> patterns = {"F\\d{1,3}", "S\\d{1,3}", "uLoM\\d{1,3}",
                                       "ACCT", "C\\d{1}"};
  std::vector<std::string> matches;
  for (const auto &entry : std::filesystem::directory_iterator(folder)) {
    if (entry.path().extension() ==
        std::filesystem::path("foo.trc").extension()) {
      const auto filename = entry.path().filename().string();
      auto match =
          std::any_of(patterns.begin(), patterns.end(),
                      [&filename](const std::string &pattern) {
                        return std::regex_search(filename, std::regex(pattern));
                      });

      for (const auto &pattern : patterns) {
        try {
          std::smatch match;
          if (std::regex_search(filename, match, std::regex(pattern))) {
            matches.push_back(match.str());
          }
        } catch (const std::regex_error &e) {
        }
      }
    }
  }
  std::unordered_set<std::string> set(matches.begin(), matches.end());
  return std::vector<std::string>(set.begin(), set.end());
}
