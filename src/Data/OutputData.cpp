#include "OutputData.h"

OutputData::OutputData() {
  reset();
}

void OutputData::operator+=(const OutputData& rhs) {
  livingCount_ += rhs.getLivingCount();
}

void OutputData::reset() {
  livingCount_ = 0;
}

const std::uint64_t& OutputData::getLivingCount() const {
  return livingCount_;
}

void OutputData::setLivingCount(const std::uint64_t livingCount) {
  livingCount_ = livingCount;
}
