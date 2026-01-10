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

#include <cstdint>
#include <stdfloat>
#include <string>
#include <vector>

class MatrixDatum {
 public:
  explicit MatrixDatum(const std::string&);
  ~MatrixDatum();

  void setGroupSize(const std::uint32_t&);

  std::float64_t getDataAtIndices(const std::uint32_t&, const std::uint32_t&) const;
  void addDataAtIndex(const std::uint32_t&, const std::float64_t&);
  void addData(const std::vector<std::float64_t>);

  std::uint32_t getRows() const;
  std::uint32_t getColumns() const;
  const std::string& getName() const;

 private:
  std::vector<std::vector<std::float64_t>> data_;
  const std::string name_;
};

#endif
