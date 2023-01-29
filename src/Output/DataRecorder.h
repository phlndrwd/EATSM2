#ifndef DATARECORDER
#define DATARECORDER

#include "Constants.h"
#include "Types.h"

class DataRecorder {
 public:
  DataRecorder();
  ~DataRecorder();

  bool Initialise(const Types::StringMatrix&);
  void InitialiseMatrix(const std::string&, const unsigned&);

  static Types::DataRecorderPointer Get();

  void AddDataTo(const std::string&, const float&);
  void AddDataTo(const std::string&, const Types::FloatVector);
  void AddDataTo(const std::string&, const unsigned&, const float&);

  void SetVectorDataOn(const std::string&, const Types::FloatVector);

  void AddInputFilePath(const std::string&);

  Types::VectorDatumMap GetVectorDatumMap() const;
  Types::MatrixDatumMap GetMatrixDatumMap() const;

  Types::StringVector GetInputFilePaths() const;

 private:
  Types::VectorDatumPointer GetVectorDatumFromName(const std::string&);
  Types::MatrixDatumPointer GetMatrixDatumFromName(const std::string&);

  static Types::DataRecorderPointer mThis;

  Types::VectorDatumMap mVectorDatumMap;
  Types::MatrixDatumMap mMatrixDatumMap;

  Types::StringMatrix mVectorDatumMetadata;
  Types::StringMatrix mMatrixDatumMetadata;

  Types::StringVector mInputFilePaths;
};

#endif
