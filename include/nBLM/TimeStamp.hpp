#pragma once

#include "nBLM/IOLecroy.hpp"

#include <array>
#include <iostream>

class TimeStamp {
public:
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  double frac_sec;

  void increment();
  std::tm create_tm() const;
  static int leap_year(const int year);
  static const std::array<int, 13> days;

public:
  TimeStamp(const int year = 1970, const int month = 1, const int day = 1,
            const int hour = 0, const int minute = 0, const int second = 0,
            const double frac_sec = 0.0);
  TimeStamp(const time_stamp ts, const Trigtime trig_time);
  TimeStamp(const TimeStamp &old) = default;
  ~TimeStamp() noexcept = default;

  friend std::ostream &operator<<(std::ostream &, const TimeStamp &);

  int end_of_month(int) const;

  void AddNanoSecond(double ns);

  double toEpochUnix() const;
  double toEpochROOT() const;
};

bool operator<(const TimeStamp &ts1, const TimeStamp &ts2);