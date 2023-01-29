#include "Strings.h"

#include <algorithm>

#include "Constants.h"

double Strings::StringToNumber(const std::string& string) {
  double number = strtod(string.c_str(), nullptr);

  return number;
}

const Types::StringVector Strings::StringToWords(const std::string& inputString, const char wordTerminationCharacter) {
  std::stringstream stringStream(inputString);

  std::string word = "";
  Types::StringVector wordList;

  while (std::getline(stringStream, word, wordTerminationCharacter)) {
    wordList.push_back(word);
  }

  return wordList;
}

const std::string Strings::DoubleToPrecisionString(const double& value, const unsigned& decimals) {
  std::ostringstream outputStringStream;
  outputStringStream << std::fixed << std::setprecision(decimals) << value;

  return outputStringStream.str();
}

int Strings::FindFirstPositionOfCharacter(const std::string inString, const char character) {
  int index = Constants::cMissingValue;

  for (unsigned charIndex = 0; charIndex < inString.length(); charIndex++) {
    if (inString[charIndex] == character) {
      index = charIndex;
      break;
    }
  }
  return index;
}

std::string Strings::TruncateStringAtCharacter(const std::string inString, const char character) {
  std::string outString = inString;

  int index = FindFirstPositionOfCharacter(outString, character);
  if (index != Constants::cMissingValue) outString = outString.substr(0, index);

  return outString;
}

std::string Strings::ToLowercase(const std::string inString) {
  std::string outString;

  std::transform(inString.begin(), inString.end(), std::back_inserter(outString), tolower);

  return outString;
}

std::string Strings::RemoveCharacter(const std::string inString, const char character) {
  std::string outString = inString;
  outString.erase(remove(outString.begin(), outString.end(), character), outString.end());

  return outString;
}

std::string Strings::RemoveWhiteSpace(const std::string inString) {
  return RemoveCharacter(inString, Constants::cWhiteSpaceCharacter);
}
