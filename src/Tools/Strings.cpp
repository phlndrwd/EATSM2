/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Strings.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "Constants.h"

template <class T>
const std::string Strings::toString(const T& input) {
  std::stringstream stringStream;
  stringStream << input;

  return stringStream.str();
}

template const std::string Strings::toString<char>(const char& input);
template const std::string Strings::toString<int>(const int& input);

double Strings::stringToNumber(const std::string& string) {
  double number = strtod(string.c_str(), nullptr);

  return number;
}

const std::vector<std::string> Strings::stringToWords(const std::string& inputString, const char wordTerminationCharacter) {
  std::stringstream stringStream(inputString);

  std::string word = "";
  std::vector<std::string> wordList;

  while (std::getline(stringStream, word, wordTerminationCharacter)) {
    wordList.push_back(word);
  }

  return wordList;
}

const std::string Strings::doubleToPrecisionString(const double& value, const std::uint32_t& decimals) {
  std::ostringstream outputStringStream;
  outputStringStream << std::fixed << std::setprecision(decimals) << value;

  return outputStringStream.str();
}

int Strings::findFirstPositionOfCharacter(const std::string inString, const char character) {
  int index = consts::kMissingValue;

  for (std::uint32_t charIndex = 0; charIndex < inString.length(); charIndex++) {
    if (inString[charIndex] == character) {
      index = charIndex;
      break;
    }
  }
  return index;
}

std::string Strings::truncateStringAtCharacter(const std::string inString, const char character) {
  std::string outString = inString;

  int index = findFirstPositionOfCharacter(outString, character);
  if (index != consts::kMissingValue) outString = outString.substr(0, index);

  return outString;
}

std::string Strings::toLowercase(const std::string inString) {
  std::string outString;

  std::transform(inString.begin(), inString.end(), std::back_inserter(outString), tolower);

  return outString;
}

std::string Strings::removeCharacter(const std::string inString, const char character) {
  std::string outString = inString;
  outString.erase(remove(outString.begin(), outString.end(), character), outString.end());

  return outString;
}

std::string Strings::removeWhiteSpace(const std::string inString) {
  return removeCharacter(inString, consts::kWhiteSpaceCharacter);
}
