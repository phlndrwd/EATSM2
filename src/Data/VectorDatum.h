#ifndef VECTORDATUM
#define VECTORDATUM

#include <string>
#include <vector>

class VectorDatum {
 public:
  VectorDatum(const std::string&);
  ~VectorDatum();

  float GetDataAtIndex(const unsigned&) const;

  void SetData(const std::vector<float>);
  void AddData(const float&);

  unsigned GetSize() const;
  const std::string& GetName() const;

 private:
  std::vector<float> mData;
  std::string mName;
};

#endif
