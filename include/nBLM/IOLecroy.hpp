#pragma once

#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <vector>

/**
 * @brief Type of the raw data
 *
 */
enum comm_type : std::int16_t { byte = 0, word = 1 };

/**
 * @brief Endianness of the raw data
 *
 */
enum comm_order : std::int16_t { HIFIRST = 0, LOFIRST = 1 };

/**
 * @brief Offset for each metadata.
 *
 */
enum offset {
  DESCRIPTOR_NAME = 0,
  TEMPLATE_NAME = 16,
  COMM_TYPE = 32,
  COMM_ORDER = 34,
  WAVE_DESCRIPTOR = 36,
  USER_TEXT = 40,
  RES_DESC1 = 44,

};

/**
 * @brief Type of acquisition.
 *
 */
enum record_type : std::int16_t {
  single_sweep = 0,
  interleaved = 1,
  histogram = 2,
  graph = 3,
  filter_coefficient = 4,
  complex = 5,
  extrema = 6,
  sequence_obsolete = 7,
  centered_RIS = 8,
  peak_detect = 9
};

/**
 * @brief Timebase used by the scope.
 *
 */
enum timebase : std::int16_t {
  ps_1 = 0,
  ps_2 = 1,
  ps_3 = 2,
  ps_10 = 3,
  ps_20 = 4,
  ps_50 = 5,
  ps_100 = 6,
  ps_200 = 7,
  ps_500 = 8,
  ns_1 = 9,
  ns_2 = 10,
  ns_5 = 11,
  ns_10 = 12,
  ns_20 = 13,
  ns_50 = 14,
  ns_100 = 15,
  ns_200 = 16,
  ns_500 = 17,
  us_1 = 18,
  us_2 = 19,
  us_5 = 20,
  us_10 = 21,
  us_20 = 22,
  us_50 = 23,
  us_100 = 24,
  us_200 = 25,
  us_500 = 26,
  ms_1 = 27,
  ms_2 = 28,
  ms_5 = 29,
  ms_10 = 30,
  ms_20 = 31,
  ms_50 = 32,
  ms_100 = 33,
  ms_200 = 34,
  ms_500 = 35,
  s_1 = 36,
  s_2 = 37,
  s_s = 38,
  s_10 = 39,
  s_20 = 40,
  s_50 = 41,
  s_100 = 42,
  s_200 = 43,
  s_500 = 44,
  ks_1 = 45,
  ks_2 = 46,
  ks_5 = 47,
  external = 100,
};

/**
 * @brief Processing mode on the scope.
 *
 */
enum processing_done : std::int16_t {
  no_processing = 0,
  fir_filter = 1,
  interpolated = 2,
  sparsed = 3,
  autoscaled = 4,
  no_result = 5,
  rolling = 6,
  cumulative = 7
};

/**
 * @brief Input channel coupling mode.
 *
 */
enum vert_coupling : std::int16_t {
  DC_50_Ohms = 0,
  ground1 = 1,
  DC_1MOhms = 2,
  ground2 = 3,
  AC_50_Ohms = 4
};

/**
 * @brief Vertical gain in the scope (V/div).
 *
 */
enum fixed_vert_gain : std::int16_t {
  uV_1 = 0,
  uV_2 = 1,
  uV_5 = 2,
  uV_10 = 3,
  uV_20 = 4,
  uV_50 = 5,
  uV_100 = 6,
  uV_200 = 7,
  uV_500 = 8,
  mV_1 = 9,
  mV_2 = 10,
  mV_5 = 11,
  mV_10 = 12,
  mV_20 = 13,
  mV_50 = 14,
  mV_100 = 15,
  mV_200 = 16,
  mV_500 = 17,
  V_1 = 18,
  V_2 = 19,
  V_s = 20,
  V_10 = 21,
  V_20 = 22,
  V_50 = 23,
  V_100 = 24,
  V_200 = 25,
  V_500 = 26,
  kV_1 = 27
};

/**
 * @brief Bandwidth filter enabled.
 *
 */
enum bandwidth_limit : std::int16_t { on = 0, off = 1 };

/**
 * @brief Channel id.
 *
 */
enum wave_source : std::int16_t {
  CHANNEL_1 = 0,
  CHANNEL_2 = 1,
  CHANNEL_3 = 2,
  CHANNEL_4 = 3,
  UNKNOWN = 9
};

/**
 * @brief timestamp struct.
 * This struct contains the time information from the scope.
 */
struct time_stamp {
  std::double_t seconds;
  std::int8_t minutes;
  std::int8_t hours;
  std::int8_t days;
  std::int8_t months;
  std::int16_t year;
  std::int16_t unused;
  /**
   * @brief Ostream insertion operator overload for Lecroy time stamp.
   * @param os Ostream object.
   * @param ts time_stamp struct from Lecroy data.
   * @return std::ostream&
   */
  friend std::ostream &operator<<(std::ostream &os, const time_stamp &ts);
};

#pragma pack(push, 1)
/**
 * @brief WAVEDESC metadata struct.
 * This struct contains all information from the scope.
 * This struct is ready in a signal pass directly in buffer.
 */
struct wavedesc {
  // Description
  std::uint8_t DESCRIPTOR_NAME[16];
  std::uint8_t TEMPLATE_NAME[16];
  comm_type COMM_TYPE;
  comm_order COMM_ORDER;
  // Blocks data
  std::int32_t WAVE_DESCRIPTOR;
  std::int32_t USER_TEXT;
  std::int32_t RES_DESC1;
  // Arrays data
  std::int32_t TRIGTIME_ARRAY;
  std::int32_t RIS_TIME_ARRAY;
  std::int32_t RES_ARRAY1;
  std::int32_t WAVE_ARRAY_1;
  std::int32_t WAVE_ARRAY_2;
  std::int32_t RES_ARRAY2;
  std::int32_t RES_ARRAY3;
  // Instrument
  std::uint8_t INSTRUMENT_NAME[16];
  std::int32_t INSTRUMENT_NUMBER;
  std::uint8_t TRACE_LABEL[16];
  std::int16_t RESERVED1;
  std::int16_t RESERVED2;
  // Waveform
  std::int32_t WAVE_ARRAY_COUNT;
  std::int32_t PNTS_PER_SCREEN;
  std::int32_t FIRST_VALID_PNT;
  std::int32_t LAST_VALID_PNT;
  std::int32_t FIRST_POINT;
  std::int32_t SPARSING_FACTOR;
  std::int32_t SEGMENT_INDEX;
  std::int32_t SUBARRAY_COUNT;
  std::int32_t SWEEPS_PER_ACQ;
  std::int16_t POINTS_PER_PAIR;
  std::int16_t PAIR_OFFSET;
  std::float_t VERTICAL_GAIN;
  std::float_t VERTICAL_OFFSET;
  std::float_t MAX_VALUE;
  std::float_t MIN_VALUE;
  std::int16_t NOMINAL_BITS;
  std::int16_t NOM_SUBARRAY_COUNT;
  std::float_t HORIZ_INTERVAL;
  std::double_t HORIZ_OFFSET;
  std::double_t PIXEL_OFFSET;
  std::uint8_t VERTUNIT[48];
  std::uint8_t HORUNIT[48];
  std::int16_t RESERVED3;
  std::int16_t RESERVED4;
  time_stamp TRIGGER_TIME;
  std::float_t ACQ_DURATION;
  record_type RECORD_TYPE;
  processing_done PROCESSING_DONE;
  std::int16_t RESERVED5;
  std::int16_t RIS_SWEEPS;
  // Acquisition condition
  timebase TIMEBASE;
  vert_coupling VERT_COUPLING;
  std::float_t PROBE_ATT;
  fixed_vert_gain FIXED_VERT_GAIN;
  bandwidth_limit BANDWIDTH_LIMIT;
  std::float_t VERTICAL_VERNIER;
  std::float_t ACQ_VERT_OFFSET;
  wave_source WAVE_SOURCE;
};
#pragma pack(pop)

#pragma pack(push, 1)
/**
 * @brief Trigger information for each signal
 *
 */
struct Trigtime {
  std::double_t TRIGGER_TIME;
  std::double_t TRIGGER_OFFSET;
};
#pragma pack(pop)

/**
 * @brief LecroyReader read metadata and data from a TRC file.
 * This object allows to read binary data from TRC file made by LeCroy scopes,
 * then the data are accessible through C++ types.
 */
class LecroyReader {
private:
  /* data */
  wavedesc metadata;
  std::vector<Trigtime> trigtimes;
  std::vector<int> raw_data;
  std::vector<double> signal;

  bool logging;

  /* Filename */
  std::string filepath;
  /* File stream */
  std::ifstream lecroy_file;
  /* File buffer */
  std::vector<std::byte> buff;
  /* */
  std::size_t size;
  /* Global offset */
  std::size_t g_offset = 0;

  void FindWavedesc();
  void ReadWavedesc();
  void ReadUsertext();
  void ReadTriggertime();
  void ReadWavedata();

  /**
   * @brief Create a signal from raw data.
   * The converted signal is computed like this: gain * raw_signal - offset
   * @param scale Scale the signal according to the gain in metadata.
   * @param offset Remove the offset according to the meatadata.
   */
  void CreateSignal(bool scale = true, bool offset = true);

public:
  /**
   * @brief Construct a new Lecroy Reader object.
   * The reader can read TRC from Lecroy scopes.
   * Currently the reader can read only basic blocs from the files.
   * @param filepath Input filepath.
   * @param read_data Read the metadata and the wave data if true.
   * @param logging Activate logging.
   */
  LecroyReader(std::string filepath, bool read_data = true,
               bool logging = false);
  ~LecroyReader();

  /**
   * @brief Get the acquisition metadata.
   *
   * @return const wavedesc& Metadata.
   */
  const wavedesc &GetMetadata() const;

  /**
   * @brief Get the raw data in binary format.
   *
   * @return const std::vector<int>& Raw data in binary format.
   */
  const std::vector<int> &GetRawdata() const;

  /**
   * @brief Get the scope signal.
   * @return const std::vector<double>& Data in double precision (scaling +
   * offset).
   */
  const std::vector<double> &GetSignal() const;

  /**
   * @brief Get the trigger time array.
   *
   * @return const std::vector<Trigtime>& Trigger array data.
   */
  const std::vector<Trigtime> &GetTrigtimes() const;

  void Print();

  /**
   * @brief Read only metadata from a file.
   *
   * @return const wavedesc& Metadata.
   */
  static const wavedesc &ReadMetaData(const std::string filepath);
};

/**
 * @brief time_stamp comparison operator overload (t1 < t2).
 * @param t1 time_stamp 1 struct from Lecroy data.
 * @param t2 time_stamp 1 struct from Lecroy data.
 * @return bool
 */
bool operator<(const time_stamp &t1, const time_stamp &t2);

/**
 * @brief Ostream insertion operator overload for Lecroy trigger time.
 *
 * @param os Ostream object.
 * @param t Trigtime struct from Lecroy data.
 * @return std::ostream&
 */
std::ostream &operator<<(std::ostream &os, const Trigtime &t);