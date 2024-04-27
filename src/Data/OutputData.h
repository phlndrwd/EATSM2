#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H

#include <cstdint>

class OutputData {
public:
  OutputData();

  void operator+=(const OutputData&);

  void reset();

  const std::uint64_t& getLivingCount() const;
  const std::uint64_t& getDeadCount() const;

  // PJU FIX - Pass copies for now. Investigate effect of passing rvalues when optimising.
  void setLivingCount(const std::uint64_t);
  void setDeadCount(const std::uint64_t);

private:
  std::uint64_t livingCount_;
  std::uint64_t deadCount_;
};

#endif // OUTPUTDATA_H
