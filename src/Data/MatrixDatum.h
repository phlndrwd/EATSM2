#ifndef MATRIXDATUM
#define MATRIXDATUM

#include <string>
#include <vector>

class MatrixDatum {
 public:
  MatrixDatum(const std::string&);
  ~MatrixDatum();

  void SetGroupSize(const unsigned&);

  float GetDataAtIndices(const unsigned&, const unsigned&) const;
  void AddDataAtIndex(const unsigned&, const float&);
  void AddData(const std::vector<float>);

  unsigned GetRows() const;
  unsigned GetColumns() const;
  const std::string& GetName() const;

 private:
  std::vector<std::vector<float>> mData;
  const std::string mName;
};

#endif
