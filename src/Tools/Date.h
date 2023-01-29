#ifndef DATE
#define DATE

#include "Constants.h"

class Date {
 public:
  static std::string GetDateAndTimeString(const std::string format = Constants::cCompleteDateFormat,
                                          unsigned addedSeconds = 0);
};

#endif
