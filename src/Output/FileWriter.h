/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef FILEWRITER
#define FILEWRITER

#include <string>

class Environment;

class FileWriter {
 public:
  FileWriter();

  void writeOutputData(Environment&);

 private:
  void initialiseOutputDirectory();
  bool writeInputFiles();

  bool writeVectorDatums();
  bool writeMatrixDatums();
  bool writeStateFile(Environment&);

  std::string outputPath_;
  std::string dataSetDirectoryName_;
};

#endif
