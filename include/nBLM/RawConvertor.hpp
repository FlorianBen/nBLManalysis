#pragma once

#include <string>
#include <vector>

/**
 * RawConvertorNTpule is a pre-processing class that read raw files and convert
 * them to a RNTuple file.
 *
 * RawConvertorNTpule convert a list of raw files into a RNTuple file. The
 * RNTuple format is the new ROOT dataform suposed to replace TTree.
 *
 */
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

  // TODO: Implement if necessary
  void AddFile(std::string filename_in);
  // TODO: Implement if necessary
  void AddFiles();

  /**
   * Perform the conversion and write data in the output file.
   */
  void Convert();

  /**
   * Set the name of the dataset in the RNTuple file.
   * \param name Name of the dataset.
   */
  void SetName(std::string name);

  /**
   * Set the name of the output file.
   * \param filename_out Name of the output file.
   */
  void SetFileNameOuput(std::string filename_out);

  /**
   * Set the name of the output folder.
   * \param folder_out Name of the output folder.
   */
  void SetFolderOutput(std::string folder_out);

  /**
   * Sort the file list by index.
   */
  void SortIndex();

  /**
   * Sort the file list using the input file metadata. The sort is done on the
   * Timestamp.
   */
  void SortMedatadata();

  void PrintMedatadata(const int index = 0);

  void PrintDateRange();

};