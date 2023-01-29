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
