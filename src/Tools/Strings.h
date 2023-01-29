#ifndef CONVERTOR
#define CONVERTOR

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Constants.h"
#include "Types.h"

class Strings {
 public:
  template <class T>
  static const std::string ToString(const T& input) {
    std::stringstream stringStream;
    stringStream << input;

    return stringStream.str();
  }

  static double StringToNumber(const std::string&);

  static const Types::StringVector StringToWords(const std::string&, const char);
  static const std::string DoubleToPrecisionString(const double&, const unsigned&);

  static int FindFirstPositionOfCharacter(const std::string, const char);
  static std::string TruncateStringAtCharacter(const std::string, const char);

  static std::string ToLowercase(const std::string);
  static std::string RemoveCharacter(const std::string, const char);
  static std::string RemoveWhiteSpace(const std::string);
};

#endif
