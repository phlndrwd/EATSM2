/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "MatrixDatum.h"

MatrixDatum::MatrixDatum(const std::string& name)
    : name_(name) {}

MatrixDatum::~MatrixDatum() {}

void MatrixDatum::setGroupSize(const std::uint32_t& size) {
  data_.resize(size);
}

float MatrixDatum::getDataAtIndices(const std::uint32_t& rowIndex, const std::uint32_t& columnIndex) const {
  return data_[columnIndex][rowIndex];
}

void MatrixDatum::addDataAtIndex(const std::uint32_t& index, const float& data) {
  data_[index].push_back(data);
}

void MatrixDatum::addData(const std::vector<float> data) {
  data_.push_back(data);
}

std::uint32_t MatrixDatum::getRows() const {
  return data_[0].size();
}

std::uint32_t MatrixDatum::getColumns() const {
  return data_.size();
}

const std::string& MatrixDatum::getName() const {
  return name_;
}
