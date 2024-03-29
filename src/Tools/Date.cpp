#include "Date.h"

#include <ctime>
#include <sys/time.h>  // For struct timeval

#include "Constants.h"

std::string Date::getDateAndTimeString(const std::string format, unsigned addedSeconds) {
  char dateTimeChar[constants::kDateTimeBufferSize];
  timeval timeNow;
  gettimeofday(&timeNow, nullptr);
  time_t rawtime = static_cast<time_t>(timeNow.tv_sec + addedSeconds);

  struct tm *timeinfo;
  timeinfo = localtime(&rawtime);

  std::strftime(dateTimeChar, constants::kDateTimeBufferSize, format.c_str(), timeinfo);
  std::string dateTime(dateTimeChar);

  return dateTime;
}
