#ifndef CONVERTOR
#define CONVERTOR

#include <sstream>
#include <string>
#include <vector>

class Strings {
 public:
  template <class T>
  static const std::string toString(const T& input);

  static double stringToNumber(const std::string&);

  static const std::vector<std::string> stringToWords(const std::string&, const char);
  static const std::string doubleToPrecisionString(const double&, const unsigned&);

  static int findFirstPositionOfCharacter(const std::string, const char);
  static std::string truncateStringAtCharacter(const std::string, const char);

  static std::string toLowercase(const std::string);
  static std::string removeCharacter(const std::string, const char);
  static std::string removeWhiteSpace(const std::string);
};

#endif
