#ifndef DATARECORDER
#define DATARECORDER

#include <map>
#include <string>
#include <vector>

class MatrixDatum;
class VectorDatum;

class DataRecorder {
 public:
  DataRecorder();
  ~DataRecorder();

  bool Initialise(const std::vector<std::vector<std::string>>&);
  void InitialiseMatrix(const std::string&, const unsigned&);

  static DataRecorder* Get();

  void AddDataTo(const std::string&, const float&);
  void AddDataTo(const std::string&, const std::vector<float>);
  void AddDataTo(const std::string&, const unsigned&, const float&);

  void SetVectorDataOn(const std::string&, const std::vector<float>);

  void AddInputFilePath(const std::string&);

  std::map<std::string, VectorDatum*> GetVectorDatumMap() const;
  std::map<std::string, MatrixDatum*> GetMatrixDatumMap() const;

  std::vector<std::string> GetInputFilePaths() const;

 private:
  VectorDatum* GetVectorDatumFromName(const std::string&);
  MatrixDatum* GetMatrixDatumFromName(const std::string&);

  static DataRecorder* mThis;

  std::map<std::string, VectorDatum*> mVectorDatumMap;
  std::map<std::string, MatrixDatum*> mMatrixDatumMap;

  std::vector<std::vector<std::string>> mVectorDatumMetadata;
  std::vector<std::vector<std::string>> mMatrixDatumMetadata;

  std::vector<std::string> mInputFilePaths;
};

#endif
