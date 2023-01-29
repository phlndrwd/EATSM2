#include "VectorDatum.h"

VectorDatum::VectorDatum(const std::string& name) {
  mName = name;
}

VectorDatum::~VectorDatum() {}

float VectorDatum::GetDataAtIndex(const unsigned& index) const {
  return mData[index];
}

void VectorDatum::SetData(const Types::FloatVector data) {
  mData = data;
}

void VectorDatum::AddData(const float& data) {
  mData.push_back(data);
}

unsigned VectorDatum::GetSize() const {
  return mData.size();
}

const std::string& VectorDatum::GetName() const {
  return mName;
}
