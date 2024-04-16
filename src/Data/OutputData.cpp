#include "OutputData.h"

OutputData::OutputData() {
  reset();
}

void OutputData::operator+=(const OutputData& rhs) {
  populationSize_ += rhs.getPopulationSize();
}

void OutputData::reset() {
  populationSize_ = 0;
}

const std::uint32_t& OutputData::getPopulationSize() const {
  return populationSize_;
}

void OutputData::setPopulationSize(const std::uint32_t populationSize) {
  populationSize_ = populationSize;
}
