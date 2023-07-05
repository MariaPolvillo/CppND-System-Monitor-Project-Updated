#include "format.h"

#include <cmath>
#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string time;

  int hours = static_cast<int>(floor(seconds / 3600.));
  seconds -= hours * 3600;

  int mins = static_cast<int>(floor(seconds / 60.));
  seconds -= mins * 60;

  string sec_prefix = seconds < 10 ? "0" : "";
  string min_prefix = mins < 10 ? "0" : "";
  string hour_prefix = hours < 10 ? "0" : "";

  time = hour_prefix + std::to_string(hours) + ":" + min_prefix +
         std::to_string(mins) + ":" + sec_prefix +
         std::to_string(static_cast<int>(seconds));

  return time;
}