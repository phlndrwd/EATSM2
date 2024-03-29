/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef FILEREADER
#define FILEREADER

#include <string>
#include <vector>

class FileReader {
 public:
  FileReader();

  void readInputFile();

 private:
  bool readTextFile(const std::string&, bool copyToOutput = true);
  void clearRawTextData();

  std::vector<std::vector<std::string>> rawTextData_;
};

#endif
