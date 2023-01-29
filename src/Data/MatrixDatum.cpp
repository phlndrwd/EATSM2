#include "MatrixDatum.h"

MatrixDatum::MatrixDatum(const std::string& name)
    : name_(name) {}

MatrixDatum::~MatrixDatum() {}

void MatrixDatum::setGroupSize(const unsigned& size) {
  data_.resize(size);
}

float MatrixDatum::getDataAtIndices(const unsigned& rowIndex, const unsigned& columnIndex) const {
  return data_[columnIndex][rowIndex];
}

void MatrixDatum::addDataAtIndex(const unsigned& index, const float& data) {
  data_[index].push_back(data);
}

void MatrixDatum::addData(const std::vector<float> data) {
  data_.push_back(data);
}

unsigned MatrixDatum::getRows() const {
  return data_[0].size();
}

unsigned MatrixDatum::getColumns() const {
  return data_.size();
}

const std::string& MatrixDatum::getName() const {
  return name_;
}
