#include "FileReader.h"

#include <fstream>
#include <iostream>

#include "DataRecorder.h"
#include "Parameters.h"
#include "Strings.h"

FileReader::FileReader() {}

FileReader::~FileReader() {}

void FileReader::readInputFile() {
  bool success = false;

  std::string parametersFile = constants::kConfigurationDirectory + constants::kInputParametersFileName;
  if (readTextFile(parametersFile))
    if (Parameters::Get()->initialise(rawTextData_))
      if (readTextFile(constants::kConfigurationDirectory + constants::kOutputParametersFileName))
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
    unsigned lineCount = 0;

    while (std::getline(fileStream, readLine)) {
      if (readLine.length() > 0) {
        if (readLine[0] != constants::kCommentCharacter) {
          readLine =
              Strings::removeWhiteSpace(Strings::truncateStringAtCharacter(readLine, constants::kCommentCharacter));
          if (lineCount > 0) {
            rawTextData_.push_back(Strings::stringToWords(readLine, constants::kDataDelimiterValue));
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
  for (unsigned rowIndex = 0; rowIndex < rawTextData_.size(); ++rowIndex) {
    rawTextData_[rowIndex].clear();
  }
  rawTextData_.clear();
}
