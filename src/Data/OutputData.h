#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H

#include <cstdint>

class OutputData {
public:
  OutputData();

  void operator+=(const OutputData&);

  void reset();

  const std::uint32_t& getPopulationSize() const;

  void setPopulationSize(const std::uint32_t);

private:
  std::uint32_t populationSize_;
};

#endif // OUTPUTDATA_H
