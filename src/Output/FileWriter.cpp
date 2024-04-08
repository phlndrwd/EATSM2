/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "FileWriter.h"

#include <sys/stat.h>

#include <fstream>
#include <ios>
#include <map>
#include <iostream>
#include <limits>

#include "Autotrophs.h"
#include "DataRecorder.h"
#include "Date.h"
#include "Environment.h"
#include "MatrixDatum.h"
#include "Nutrient.h"
#include "Parameters.h"
#include "Strings.h"
#include "VectorDatum.h"

FileWriter::FileWriter() {
  initialiseOutputDirectory();
  if (!writeInputFiles()) {
    std::cout << "ERROR> Could not write input files to \"" << outputPath_ << "\". System exiting..." << std::endl;
    exit(1);
  }
}

void FileWriter::initialiseOutputDirectory() {
  // output/
  outputPath_ = consts::kOutputDirectoryName;
  mkdir(outputPath_.c_str(), consts::kOutputFolderPermissions);
  outputPath_.append(Strings::toString(consts::kFolderDelimiter));

  // output/[version]/
  outputPath_.append(consts::kSystemVersion);
  mkdir(outputPath_.c_str(), consts::kOutputFolderPermissions);
  outputPath_.append(Strings::toString(consts::kFolderDelimiter));

  // output/[version]/[experiment]/[date and time]
  dataSetDirectoryName_ = Date::getDateAndTimeString(consts::kDataSetNameFormat);

  outputPath_.append(dataSetDirectoryName_);
  std::int32_t returnValue = mkdir(outputPath_.c_str(), consts::kOutputFolderPermissions);

  // The following code ensures the data are written into a unique subdirectory.
  if (returnValue == -1) {
    outputPath_.append("_");
    std::int32_t stringLength = outputPath_.length();

    std::int32_t count = 1;
    while (returnValue == -1) {
      outputPath_.replace(stringLength, 1, Strings::toString(count));
      returnValue = mkdir(outputPath_.c_str(), consts::kOutputFolderPermissions);
      ++count;
    }
  }

  outputPath_.append(Strings::toString(consts::kFolderDelimiter));
  std::cout << "Output directory initialised at \"" << outputPath_ << "\"." << std::endl;
}

bool FileWriter::writeInputFiles() {
  std::vector<std::string> inputFilePaths = DataRecorder::get()->getInputFilePaths();

  for (std::uint32_t stringIndex = 0; stringIndex < inputFilePaths.size(); ++stringIndex) {
    std::ifstream sourceFileStream(inputFilePaths[stringIndex].c_str(), std::ios::in);

    std::string outputFilePath = outputPath_;
    std::vector<std::string> inputFilePathComponents =
        Strings::stringToWords(inputFilePaths[stringIndex], consts::kFolderDelimiter);

    std::string fileName = inputFilePathComponents[inputFilePathComponents.size() - 1];
    outputFilePath.append(fileName);

    std::ofstream destinationFileStream(outputFilePath.c_str(), std::ios::out);
    if (destinationFileStream.is_open()) {
      destinationFileStream << sourceFileStream.rdbuf();
      sourceFileStream.close();
      destinationFileStream.close();
    } else {
      return false;
    }
  }
  return true;
}

void FileWriter::writeOutputData(Environment& environment) {
  bool success = false;

  if (writeVectorDatums())
    success = writeMatrixDatums();

  if (success)
    std::cout << "Output data written to \"" << outputPath_ << "\"." << std::endl;
  else {
    std::cout << "ERROR> File writing failed. Could not access \"" << outputPath_ << "\"." << std::endl;
    exit(1);
  }
}

bool FileWriter::writeVectorDatums() {
  std::map<std::string, VectorDatum*> vectorDatumMap = DataRecorder::get()->getVectorDatumMap();

  for (std::map<std::string, VectorDatum*>::iterator iter = vectorDatumMap.begin(); iter != vectorDatumMap.end(); ++iter) {
    std::string fileName = iter->first;
    VectorDatum* vectorDatum = iter->second;
    std::uint32_t datumSize = vectorDatum->getSize();
    if (datumSize > 0) {
      fileName.insert(0, outputPath_).append(consts::kFileNameExtension);
      std::ofstream outputFileStream;
      outputFileStream.open(fileName.c_str(), std::ios::out);

      if (outputFileStream.is_open() == true) {
        for (std::uint32_t dataIndex = 0; dataIndex < datumSize - 1; ++dataIndex) {
          outputFileStream << vectorDatum->getDataAtIndex(dataIndex) << consts::kDataDelimiterValue;
        }
        outputFileStream << vectorDatum->getDataAtIndex(vectorDatum->getSize() - 1);
        outputFileStream.close();
      } else {
        return false;
      }
    }
  }
  return true;
}

bool FileWriter::writeMatrixDatums() {
  std::map<std::string, MatrixDatum*> matrixDatumMap = DataRecorder::get()->getMatrixDatumMap();
  for (std::map<std::string, MatrixDatum*>::iterator iter = matrixDatumMap.begin(); iter != matrixDatumMap.end(); ++iter) {
    std::string fileName = iter->first;
    MatrixDatum* matrixDatum = iter->second;
    std::uint32_t rowSize = matrixDatum->getRows();
    if (rowSize > 0) {
      fileName.insert(0, outputPath_).append(consts::kFileNameExtension);
      std::ofstream outputFileStream;
      outputFileStream.open(fileName.c_str(), std::ios::out);
      if (outputFileStream.is_open() == true) {
        for (std::uint32_t rowIndex = 0; rowIndex < rowSize; ++rowIndex) {
          for (std::uint32_t columnIndex = 0; columnIndex < matrixDatum->getColumns() - 1; ++columnIndex) {
            outputFileStream << matrixDatum->getDataAtIndices(rowIndex, columnIndex) << consts::kDataDelimiterValue;
          }
          outputFileStream << matrixDatum->getDataAtIndices(rowIndex, matrixDatum->getColumns() - 1) << std::endl;
        }
        outputFileStream.close();
      } else
        return false;
    }
  }
  return true;
}
