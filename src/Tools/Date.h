#ifndef DATE
#define DATE

#include "Constants.h"

class Date {
 public:
  static std::string getDateAndTimeString(const std::string format = constants::kCompleteDateFormat,
                                          unsigned addedSeconds = 0);
};

#endif
