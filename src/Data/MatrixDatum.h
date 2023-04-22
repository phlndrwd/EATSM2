#ifndef MATRIXDATUM
#define MATRIXDATUM

#include <string>
#include <vector>

class MatrixDatum {
 public:
  explicit MatrixDatum(const std::string&);
  ~MatrixDatum();

  void setGroupSize(const unsigned&);

  float getDataAtIndices(const unsigned&, const unsigned&) const;
  void addDataAtIndex(const unsigned&, const float&);
  void addData(const std::vector<float>);

  unsigned getRows() const;
  unsigned getColumns() const;
  const std::string& getName() const;

 private:
  std::vector<std::vector<float>> data_;
  const std::string name_;
};

#endif
