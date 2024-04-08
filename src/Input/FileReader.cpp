/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "FileReader.h"

#include <cstdint>
#include <fstream>
#include <iostream>

#include "Constants.h"
#include "DataRecorder.h"
#include "Strings.h"

FileReader::FileReader() {}

void FileReader::setParameters(Parameters& params) {
  bool success = false;

  std::string parametersFile = consts::kConfigurationDirectory + consts::kInputParametersFileName;
  if (readTextFile(parametersFile))
    if (params.initialise(rawTextData_))
      if (readTextFile(consts::kConfigurationDirectory + consts::kOutputParametersFileName))
        success = DataRecorder::get()->initialise(rawTextData_);

  if (success) {
    std::cout << "Files read successfully..." << std::endl;
  } else {
    std::cout << "ERROR> File reading failed. System exiting..." << std::endl;
    exit(1);
  }
}

bool FileReader::readTextFile(const std::string& filePath, bool copyToOutput) {
  std::cout << "Reading text file \"" << filePath << "\"..." << std::endl;
  clearRawTextData();
  std::ifstream fileStream(filePath.c_str(), std::ios::in);

  if (fileStream.is_open()) {
    std::string readLine;
    std::uint32_t lineCount = 0;

    while (std::getline(fileStream, readLine)) {
      if (readLine.length() > 0) {
        if (readLine[0] != consts::kCommentCharacter) {
          readLine =
              Strings::removeWhiteSpace(Strings::truncateStringAtCharacter(readLine, consts::kCommentCharacter));
          if (lineCount > 0) {
            rawTextData_.push_back(Strings::stringToWords(readLine, consts::kDataDelimiterValue));
          }
          lineCount++;
        }
      }
    }

    fileStream.close();
  } else
    std::cout << "File path \"" << filePath << "\" is invalid." << std::endl;

  if (copyToOutput == true) DataRecorder::get()->addInputFilePath(filePath);

  return rawTextData_.size() > 0;
}

void FileReader::clearRawTextData() {
  for (std::uint32_t rowIndex = 0; rowIndex < rawTextData_.size(); ++rowIndex) {
    rawTextData_[rowIndex].clear();
  }
  rawTextData_.clear();
}
