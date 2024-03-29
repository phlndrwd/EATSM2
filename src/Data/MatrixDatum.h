/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef MATRIXDATUM
#define MATRIXDATUM

#include <string>
#include <vector>

class MatrixDatum {
 public:
  explicit MatrixDatum(const std::string&);
  ~MatrixDatum();

  void setGroupSize(const unsigned&);

  float getDataAtIndices(const unsigned&, const unsigned&) const;
  void addDataAtIndex(const unsigned&, const float&);
  void addData(const std::vector<float>);

  unsigned getRows() const;
  unsigned getColumns() const;
  const std::string& getName() const;

 private:
  std::vector<std::vector<float>> data_;
  const std::string name_;
};

#endif
