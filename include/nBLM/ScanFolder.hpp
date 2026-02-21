#pragma once

#include <string>
#include <vector>

/**
 * ScanFolder is an helper class that list all the Lecroy trc files inside a
 * folder.
 *
 * The Lecroy scope save data in file according to this pattern:
 * ChannelName-PrefixPath-Filenumber. Channel name is the name of channel set
 * in the scope setting. Default values are C1 for channel C2 for channel 2,
 * etc. Prefix is the intermediate string set by the user, same for all the
 * channels. Finally the file number inside the sequence is given at the end.
 * This number is usually 5 digit chars padded with zeros.
 */
class ScanFolder {
private:
  std::string folder;
  std::string channel;
  std::string prefix;
  std::vector<std::pair<int, std::string>> indexs;

  /**
   * The internal function that performs the scan.
   */
  void DoScan();

  /**
   * The internal function that sort the files based on their indexes.
   */
  void SortIndex();

public:
  /**
   * Default constructor
   *
   * \param folder Folder to scan.
   * \param channel Channel name to match.
   * \param prefix Prefix string between the channel name and the file number
   * channel.
   */
  ScanFolder(std::string folder, std::string channel, std::string prefix);

  /**
   * Get the indexes of the sorted index with the related filename.
   *
   * \return The list as a vector of pair<int,string>.
   */
  std::vector<std::pair<int, std::string>> GetIndex();
};
