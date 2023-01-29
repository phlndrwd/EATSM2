#ifndef VECTORDATUM
#define VECTORDATUM

#include <string>
#include <vector>

class VectorDatum {
 public:
  VectorDatum(const std::string&);
  ~VectorDatum();

  float getDataAtIndex(const unsigned&) const;

  void setData(const std::vector<float>);
  void addData(const float&);

  unsigned getSize() const;
  const std::string& getName() const;

 private:
  std::vector<float> data_;
  std::string name_;
};

#endif
