#include "nBLM/IOLecroy.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>

LecroyReader::LecroyReader(std::string filepath, bool read_data, bool logging)
    : filepath(filepath),
      lecroy_file(filepath,
                  std::fstream::binary | std::fstream::in | std::fstream::ate),
      logging(logging) {
  if (!lecroy_file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }

  auto end = lecroy_file.tellg();
  lecroy_file.seekg(0, std::ios::beg);
  size = std::size_t(end - lecroy_file.tellg());
  buff.clear();
  buff.resize(size);

  lecroy_file.seekg(0, std::ios::beg);
  if (!lecroy_file.read((char *)buff.data(), buff.size()))
    throw std::runtime_error(filepath + ": " + std::strerror(errno));
  FindWavedesc();
  ReadWavedesc();
  if (read_data) {
    ReadUsertext();
    ReadTriggertime();
    ReadWavedata();
    CreateSignal();
  }
}

LecroyReader::~LecroyReader() { lecroy_file.close(); }

void LecroyReader::FindWavedesc() {
  std::uint8_t buff_temp[9];
  while (std::strcmp((char *)&buff_temp, "WAVEDESC")) {
    lecroy_file.seekg(g_offset++, std::ios::beg);
    lecroy_file.read((char *)&buff_temp, 9);
  }
  --g_offset;
}

void LecroyReader::ReadWavedesc() {
  std::memcpy(&metadata, &buff[0] + g_offset, sizeof(metadata));
}

void LecroyReader::ReadUsertext() {
  if (metadata.USER_TEXT < 1) {
    return;
  } else {
  }
}

void LecroyReader::ReadTriggertime() {
  trigtimes.clear();
  if (metadata.TRIGTIME_ARRAY) {
    trigtimes.resize(metadata.TRIGTIME_ARRAY / sizeof(Trigtime));
    std::memcpy(&trigtimes[0],
                &buff[0] + g_offset + metadata.WAVE_DESCRIPTOR +
                    metadata.USER_TEXT,
                metadata.TRIGTIME_ARRAY);
  } else {
    trigtimes.push_back(Trigtime{0.0, 0.0});
  }
}

void LecroyReader::ReadWavedata() {
  std::int16_t *wave_buffer16 = nullptr;
  std::int8_t *wave_buffer8 = nullptr;

  raw_data.clear();
  raw_data.resize(metadata.WAVE_ARRAY_COUNT);

  switch (metadata.COMM_TYPE) {
  case comm_type::byte:
    wave_buffer8 = new std::int8_t[metadata.WAVE_ARRAY_COUNT];
    std::memcpy(wave_buffer16,
                &buff[0] + g_offset + metadata.WAVE_DESCRIPTOR +
                    metadata.USER_TEXT + metadata.TRIGTIME_ARRAY,
                1 * metadata.WAVE_ARRAY_COUNT);
    for (auto i = 0; i < metadata.WAVE_ARRAY_COUNT; i++) {
      raw_data[i] = static_cast<int>(wave_buffer8[i]);
    }
    break;
  case comm_type::word:
    wave_buffer16 = new std::int16_t[metadata.WAVE_ARRAY_COUNT];
    std::memcpy(wave_buffer16,
                &buff[0] + g_offset + metadata.WAVE_DESCRIPTOR +
                    metadata.USER_TEXT + metadata.TRIGTIME_ARRAY,
                2 * metadata.WAVE_ARRAY_COUNT);
    for (auto i = 0; i < metadata.WAVE_ARRAY_COUNT; i++) {
      raw_data[i] = static_cast<int>(wave_buffer16[i]);
    }
    break;
  default:
    // std::cout << std::format("Invalid COMM_TYPE");
    throw std::runtime_error("Invalid COMM_TYPE");
    break;
  }

  if (wave_buffer16)
    delete[] wave_buffer16;
  if (wave_buffer8)
    delete[] wave_buffer8;
}

void LecroyReader::CreateSignal(bool scale, bool offset) {
  signal.clear();
  signal.resize(raw_data.size());
  std::transform(std::begin(raw_data), std::end(raw_data), std::begin(signal),
                 [this, scale, offset](auto ele) -> double {
                   auto res = 0.0;
                   res = (double)ele;
                   if (scale)
                     res *= this->metadata.VERTICAL_GAIN;
                   if (offset)
                     res -= this->metadata.VERTICAL_OFFSET;
                   return res;
                 });
}

const wavedesc &LecroyReader::GetMetadata() const { return metadata; }

const std::vector<int> &LecroyReader::GetRawdata() const { return raw_data; }

const std::vector<double> &LecroyReader::GetSignal() const { return signal; }

const std::vector<Trigtime> &LecroyReader::GetTrigtimes() const {
  return trigtimes;
}

const wavedesc &LecroyReader::ReadMetaData(const std::string filepath) {
  LecroyReader static_reader(filepath, false, false);
  return static_reader.GetMetadata();
}

std::ostream &operator<<(std::ostream &os, const time_stamp &ts) {
  os << ts.year << '/' << (int)ts.months << '/' << (int)ts.days << " "
     << (int)ts.hours << ":" << (int)ts.minutes << ":" << ts.seconds;
  return os;
}

bool operator<(const time_stamp &t1, const time_stamp &t2) {
  if (t1.year < t2.year)
    return true;
  else if (t1.year == t2.year) {
    if (t1.months < t2.months)
      return true;
    else if (t1.months == t2.months) {
      if (t1.days < t2.days)
        return true;
      else if (t1.days == t2.days) {
        if (t1.hours < t2.hours)
          return true;
        else if (t1.hours == t2.hours) {
          if (t1.minutes < t2.minutes)
            return true;
          else if (t1.minutes == t2.minutes) {
            if (t1.seconds < t2.seconds)
              return true;
          }
        }
      }
    }
  }
  return false;
}

std::ostream &operator<<(std::ostream &os, const Trigtime &t) {
  os << t.TRIGGER_TIME << " + " << t.TRIGGER_OFFSET;
  return os;
}

std::ostream &operator<<(std::ostream &os, const wavedesc &metadata) {
  os << "DESCRIPTOR_NAME: " << (char *)metadata.DESCRIPTOR_NAME << std::endl;
  os << "TEMPLATE_NAME: " << (char *)metadata.TEMPLATE_NAME << std::endl;
  os << "COMM_TYPE: " << metadata.COMM_TYPE << std::endl;
  os << "COMM_ORDER: " << metadata.COMM_ORDER << std::endl;
  os << "WAVE_DESCRIPTOR: " << metadata.WAVE_DESCRIPTOR << std::endl;
  os << "USER_TEXT: " << metadata.USER_TEXT << std::endl;
  os << "RES_DESC1: " << metadata.RES_DESC1 << std::endl;
  os << "TRIGTIME_ARRAY: " << metadata.TRIGTIME_ARRAY << std::endl;
  os << "RIS_TIME_ARRAY: " << metadata.RIS_TIME_ARRAY << std::endl;
  os << "RES_ARRAY1: " << metadata.RES_ARRAY1 << std::endl;
  os << "WAVE_ARRAY_1: " << metadata.WAVE_ARRAY_1 << std::endl;
  os << "WAVE_ARRAY_2: " << metadata.WAVE_ARRAY_2 << std::endl;
  os << "RES_ARRAY2: " << metadata.RES_ARRAY2 << std::endl;
  os << "RES_ARRAY3: " << metadata.RES_ARRAY3 << std::endl;
  os << "INSTRUMENT_NAME: " << metadata.INSTRUMENT_NAME << std::endl;
  os << "INSTRUMENT_NUMBER: " << metadata.INSTRUMENT_NUMBER << std::endl;
  os << "TRACE_LABEL: " << metadata.TRACE_LABEL << std::endl;
  os << "RESERVED1: " << metadata.RESERVED1 << std::endl;
  os << "RESERVED2: " << metadata.RESERVED2 << std::endl;
  os << "WAVE_ARRAY_COUNT: " << metadata.WAVE_ARRAY_COUNT << std::endl;
  os << "PNTS_PER_SCREEN: " << metadata.PNTS_PER_SCREEN << std::endl;
  os << "FIRST_VALID_PNT: " << metadata.FIRST_VALID_PNT << std::endl;
  os << "LAST_VALID_PNT: " << metadata.LAST_VALID_PNT << std::endl;
  os << "FIRST_POINT: " << metadata.FIRST_POINT << std::endl;
  os << "SPARSING_FACTOR: " << metadata.SPARSING_FACTOR << std::endl;
  os << "SEGMENT_INDEX: " << metadata.SEGMENT_INDEX << std::endl;
  os << "SUBARRAY_COUNT: " << metadata.SUBARRAY_COUNT << std::endl;
  os << "SWEEPS_PER_ACQ: " << metadata.SWEEPS_PER_ACQ << std::endl;
  os << "POINTS_PER_PAIR: " << metadata.POINTS_PER_PAIR << std::endl;
  os << "PAIR_OFFSET: " << metadata.PAIR_OFFSET << std::endl;
  os << "VERTICAL_GAIN: " << metadata.VERTICAL_GAIN << std::endl;
  os << "VERTICAL_OFFSET: " << metadata.VERTICAL_OFFSET << std::endl;
  os << "MAX_VALUE: " << metadata.MAX_VALUE << std::endl;
  os << "MIN_VALUE: " << metadata.MIN_VALUE << std::endl;
  os << "NOMINAL_BITS: " << metadata.NOMINAL_BITS << std::endl;
  os << "NOM_SUBARRAY_COUNT: " << metadata.NOM_SUBARRAY_COUNT << std::endl;
  os << "HORIZ_INTERVAL: " << metadata.HORIZ_INTERVAL << std::endl;
  os << "HORIZ_OFFSET: " << metadata.HORIZ_OFFSET << std::endl;
  os << "VERTUNIT: " << metadata.VERTUNIT << std::endl;
  os << "HORUNIT: " << metadata.HORUNIT << std::endl;
  os << "RESERVED3: " << metadata.RESERVED3 << std::endl;
  os << "RESERVED4: " << metadata.RESERVED4 << std::endl;
  os << "TRIGGER_TIME: " << metadata.TRIGGER_TIME << std::endl;
  os << "ACQ_DURATION: " << metadata.ACQ_DURATION << std::endl;
  os << "RECORD_TYPE: " << metadata.RECORD_TYPE << std::endl;
  os << "PROCESSING_DONE: " << metadata.PROCESSING_DONE << std::endl;
  os << "RESERVED5: " << metadata.RESERVED5 << std::endl;
  os << "RIS_SWEEPS: " << metadata.RIS_SWEEPS << std::endl;
  os << "TIMEBASE: " << metadata.TIMEBASE << std::endl;
  os << "VERT_COUPLING: " << metadata.VERT_COUPLING << std::endl;
  os << "PROBE_ATT: " << metadata.PROBE_ATT << std::endl;
  os << "FIXED_VERT_GAIN: " << metadata.FIXED_VERT_GAIN << std::endl;
  os << "BANDWIDTH_LIMIT: " << metadata.BANDWIDTH_LIMIT << std::endl;
  os << "VERTICAL_VERNIER: " << metadata.VERTICAL_VERNIER << std::endl;
  os << "ACQ_VERT_OFFSET: " << metadata.ACQ_VERT_OFFSET << std::endl;
  os << "WAVE_SOURCE: " << metadata.WAVE_SOURCE;
  return os;
}

std::chrono::duration<double, std::ratio<1>>
trc_time_stamp_std(const time_stamp &ts) {
  using c_days = std::chrono::duration<int, std::ratio<86400>>;
  using c_months = std::chrono::duration<int, std::ratio<2629746>>;
  using c_years = std::chrono::duration<int, std::ratio<31556952>>;
  auto time_std = // std::chrono::duration<double, std::ratio<1>>(ts.seconds) +
      std::chrono::minutes(ts.minutes) + std::chrono::hours(ts.hours) +
      c_days(ts.days) + c_months(ts.months) + c_years(ts.year);
  return time_std;
}

double trc_time_stamp_epoch_unix(const time_stamp &ts) {
  auto epoch = 0.0;
  epoch += ts.seconds;
  epoch += ts.minutes * 60.0;
  epoch += ts.hours * 3600.0;
  epoch += ts.days * 86400;
  epoch += (ts.months - 1) * 2629746;
  epoch += (ts.year - 1995) * 31556952;
  return epoch;
}

double trc_time_stamp_epoch_root(const time_stamp &ts) { return 0.0; }