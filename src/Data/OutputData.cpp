#include "OutputData.h"

OutputData::OutputData() {
  reset();
}

void OutputData::operator+=(const OutputData& rhs) {
  livingCount_ += rhs.getLivingCount();
  deadCount_ += rhs.getDeadCount();
}

void OutputData::reset() {
  livingCount_ = 0;
  deadCount_ = 0;
}

const std::uint64_t& OutputData::getLivingCount() const {
  return livingCount_;
}

const std::uint64_t& OutputData::getDeadCount() const {
  return deadCount_;
}

void OutputData::setLivingCount(const std::uint64_t livingCount) {
  livingCount_ = livingCount;
}

void OutputData::setDeadCount(const std::uint64_t deadCount) {
  deadCount_ = deadCount;
}
