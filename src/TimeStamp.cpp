#include "nBLM/TimeStamp.hpp"

#include "nBLM/nBLMUtils.hpp"

const std::array<int, 13> TimeStamp::days = {0,  31, 28, 31, 30, 31, 30,
                                             31, 31, 30, 31, 30, 31};

TimeStamp::TimeStamp(const int year, const int month, const int day,
                     const int hour, const int minute, const int second,
                     const double frac_sec)
    : year(year), month(month), day(day), hour(hour), minute(minute),
      second(second), frac_sec(frac_sec) {}

TimeStamp::TimeStamp(const time_stamp its, const Trigtime trig_time) {
  time_stamp ts = its;

  auto nsec = (ts.seconds + trig_time.TRIGGER_TIME - trig_time.TRIGGER_OFFSET);

  month = ts.months;
  year = ts.year;
  day = ts.days;

  // I assume that time offset is always smaller than 1 minute
  if (nsec < 0.0) {
    ts.minutes--;
    nsec = 60.0 + nsec;
  }

  double temp_s;
  frac_sec = std::modf(nsec, &temp_s);
  second = int(temp_s);

  if (second >= 60) {
    auto rem_s = second % 60;
    auto min = second / 60;
    second = rem_s;
    ts.minutes += min;
  } else {
    second = second;
  }
  if (ts.minutes >= 60) {
    auto rem_m = ts.minutes % 60;
    auto hours = ts.minutes / 60;
    minute = rem_m;
    ts.hours += hours;
  } else {
    minute = ts.minutes;
  }
  if (ts.hours >= 24) {
    auto rem = ts.hours % 24;
    auto days = ts.hours / 24;
    hour = rem;
    ts.days += days;
    for (int i = 1; i <= days; i++) {
      increment();
    }
  } else {
    hour = ts.hours;
  }
}

std::tm TimeStamp::create_tm() const {
  std::tm date;
  date.tm_sec = second;
  date.tm_min = minute;
  date.tm_hour = hour;
  date.tm_mday = day;
  date.tm_mon = month - 1;
  date.tm_year = year - 1900;
  return date;
}

bool operator<(const TimeStamp &t1, const TimeStamp &t2) {
  if (t1.year < t2.year)
    return true;
  else if (t1.year == t2.year) {
    if (t1.month < t2.month)
      return true;
    else if (t1.month == t2.month) {
      if (t1.days < t2.days)
        return true;
      else if (t1.days == t2.days) {
        if (t1.hour < t2.hour)
          return true;
        else if (t1.hour == t2.hour) {
          if (t1.minute < t2.minute)
            return true;
          else if (t1.minute == t2.minute) {
            if (t1.second < t2.second)
              return true;
            else if (t1.second == t2.second) {
              if (t1.frac_sec < t2.frac_sec)
                return true;
            }
          }
        }
      }
    }
  }
  return false;
}

int TimeStamp::end_of_month(int dd) const {
  if (month == 2 && leap_year(year))
    return dd == 29;
  else
    return dd == days[month];
}

void TimeStamp::increment() {
  if (end_of_month(day) && month == 12) {
    day = 1;
    month = 1;
    year++;
  } else if (end_of_month(day)) {
    day = 1;
    month++;
  } else
    day++;
}

int TimeStamp::leap_year(const int yy) {
  if (yy % 400 == 0 || (yy % 100 != 0 && yy % 4 == 0))
    return 1; //  leap year
  else
    return 0; //  not a leap year
}

double TimeStamp::toEpochUnix() const {
  auto epoch = 0.0;
  auto temp_tm = create_tm();
  epoch = std::mktime(&temp_tm) + frac_sec;
  return epoch + frac_sec;
}

double TimeStamp::toEpochROOT() const {
  return toEpochUnix() + 75.0 * 31556926;
}

std::ostream &operator<<(std::ostream &os, const TimeStamp &ts) {
  std::cout.precision(9);
  os << ts.year << '/' << zfill((int)ts.month, 2) << '/'
     << zfill((int)ts.day, 2) << " " << (int)ts.hour << ":"
     << zfill((int)ts.minute, 2) << ":" << zfill((int)ts.second, 2) << " + "
     << std::fixed << ts.frac_sec;
  return os;
}

TimeStamp operator+(int numdays, const TimeStamp &dd) {
  TimeStamp newdate = dd;
  for (int i = 1; i <= numdays; i++)
    newdate.increment();
  return newdate;
}

TimeStamp operator+(const TimeStamp &dd, int numdays) {
  TimeStamp newdate = dd;
  for (int i = 1; i <= numdays; i++)
    newdate.increment();
  return newdate;
}