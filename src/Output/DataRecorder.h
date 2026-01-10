/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef DATARECORDER
#define DATARECORDER

#include <map>
#include <cstdint>
#include <stdfloat>
#include <string>
#include <vector>

class MatrixDatum;
class VectorDatum;

class DataRecorder {
 public:
  DataRecorder();
  ~DataRecorder();

  bool initialise(const std::vector<std::vector<std::string>>&);
  void initialiseMatrix(const std::string&, const std::uint32_t&);

  static DataRecorder* get();

  void addDataTo(const std::string&, const std::float64_t&);
  void addDataTo(const std::string&, const std::vector<std::float64_t>);
  void addDataTo(const std::string&, const std::uint32_t&, const std::float64_t&);

  void setVectorDataOn(const std::string&, const std::vector<std::float64_t>);

  void addInputFilePath(const std::string&);

  std::map<std::string, VectorDatum*> getVectorDatumMap() const;
  std::map<std::string, MatrixDatum*> getMatrixDatumMap() const;

  std::vector<std::string> getInputFilePaths() const;

 private:
  VectorDatum* getVectorDatumFromName(const std::string&);
  MatrixDatum* getMatrixDatumFromName(const std::string&);

  static DataRecorder* this_;

  std::map<std::string, VectorDatum*> vectorDatumMap_;
  std::map<std::string, MatrixDatum*> matrixDatumMap_;

  std::vector<std::vector<std::string>> vectorDatumMetadata_;
  std::vector<std::vector<std::string>> matrixDatumMetadata_;

  std::vector<std::string> inputFilePaths_;
};

#endif
