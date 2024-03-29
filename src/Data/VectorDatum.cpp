/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "VectorDatum.h"

VectorDatum::VectorDatum(const std::string& name) {
  name_ = name;
}

VectorDatum::~VectorDatum() {}

float VectorDatum::getDataAtIndex(const unsigned& index) const {
  return data_[index];
}

void VectorDatum::setData(const std::vector<float> data) {
  data_ = data;
}

void VectorDatum::addData(const float& data) {
  data_.push_back(data);
}

unsigned VectorDatum::getSize() const {
  return data_.size();
}

const std::string& VectorDatum::getName() const {
  return name_;
}
