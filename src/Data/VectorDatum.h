/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef VECTORDATUM
#define VECTORDATUM

#include <cstdint>
#include <string>
#include <vector>

class VectorDatum {
 public:
  explicit VectorDatum(const std::string&);
  ~VectorDatum();

  float getDataAtIndex(const std::uint32_t&) const;

  void setData(const std::vector<float>);
  void addData(const float&);

  std::uint32_t getSize() const;
  const std::string& getName() const;

 private:
  std::vector<float> data_;
  std::string name_;
};

#endif
