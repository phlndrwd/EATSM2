#include "DataRecorder.h"

#include "Constants.h"
#include "MatrixDatum.h"
#include "Parameters.h"
#include "Strings.h"
#include "VectorDatum.h"

Types::DataRecorderPointer DataRecorder::mThis = nullptr;

Types::DataRecorderPointer DataRecorder::Get() {
  if (mThis == nullptr) mThis = new DataRecorder();

  return mThis;
}

DataRecorder::DataRecorder() {}

DataRecorder::~DataRecorder() {
  for (Types::VectorDatumMap::iterator iter = mVectorDatumMap.begin(); iter != mVectorDatumMap.end(); ++iter) {
    delete iter->second;
  }
  for (Types::MatrixDatumMap::iterator iter = mMatrixDatumMap.begin(); iter != mMatrixDatumMap.end(); ++iter) {
    delete iter->second;
  }
  if (mThis != nullptr) delete mThis;
}

bool DataRecorder::Initialise(const Types::StringMatrix& rawOutputParameterData) {
  if (rawOutputParameterData.size() > 0) {
    for (unsigned rowIndex = 0; rowIndex < rawOutputParameterData.size(); ++rowIndex) {
      std::string name = Strings::RemoveWhiteSpace(rawOutputParameterData[rowIndex][Constants::eDatumName]);
      std::string type =
          Strings::RemoveWhiteSpace(Strings::ToLowercase(rawOutputParameterData[rowIndex][Constants::eDatumType]));

      Types::StringVector datumMetadata;
      datumMetadata.push_back(name);
      datumMetadata.push_back(type);

      if (type == Constants::cVectorDatumTypeName) {
        mVectorDatumMetadata.push_back(datumMetadata);
      } else if (type == Constants::cMatrixDatumTypeName) {
        mMatrixDatumMetadata.push_back(datumMetadata);
      }
    }
    return true;
  } else {
    return false;
  }
}

void DataRecorder::InitialiseMatrix(const std::string& name, const unsigned& size) {
  Types::MatrixDatumPointer matrixDatum = GetMatrixDatumFromName(name);

  if (matrixDatum != nullptr) matrixDatum->SetGroupSize(size);
}

void DataRecorder::AddDataTo(const std::string& name, const float& data) {
  Types::VectorDatumPointer vectorDatum = GetVectorDatumFromName(name);

  if (vectorDatum != nullptr) vectorDatum->AddData(data);
}

void DataRecorder::AddDataTo(const std::string& name, const Types::FloatVector data) {
  Types::MatrixDatumPointer matrixDatum = GetMatrixDatumFromName(name);

  if (matrixDatum != nullptr) matrixDatum->AddData(data);
}

void DataRecorder::AddDataTo(const std::string& name, const unsigned& index, const float& data) {
  Types::MatrixDatumPointer matrixDatum = GetMatrixDatumFromName(name);

  if (matrixDatum != nullptr) matrixDatum->AddDataAtIndex(index, data);
}

void DataRecorder::SetVectorDataOn(const std::string& name, const Types::FloatVector data) {
  Types::VectorDatumPointer vectorDatum = GetVectorDatumFromName(name);

  if (vectorDatum != nullptr) vectorDatum->SetData(data);
}

void DataRecorder::AddInputFilePath(const std::string& inputFilePath) { mInputFilePaths.push_back(inputFilePath); }

Types::VectorDatumMap DataRecorder::GetVectorDatumMap() const { return mVectorDatumMap; }

Types::MatrixDatumMap DataRecorder::GetMatrixDatumMap() const { return mMatrixDatumMap; }

Types::StringVector DataRecorder::GetInputFilePaths() const { return mInputFilePaths; }

Types::VectorDatumPointer DataRecorder::GetVectorDatumFromName(const std::string& name) {
  Types::VectorDatumPointer vectorDatum = nullptr;
  Types::VectorDatumMap::iterator iter = mVectorDatumMap.find(name);

  if (iter != mVectorDatumMap.end()) {
    vectorDatum = iter->second;
  } else {
    for (unsigned datumIndex = 0; datumIndex < mVectorDatumMetadata.size(); ++datumIndex) {
      std::string datumName = mVectorDatumMetadata[datumIndex][Constants::eDatumName];

      if (datumName == name) {
        vectorDatum = new VectorDatum(datumName);
        mVectorDatumMap.insert(std::pair<std::string, Types::VectorDatumPointer>(datumName, vectorDatum));
        break;
      }
    }
  }
  return vectorDatum;
}

Types::MatrixDatumPointer DataRecorder::GetMatrixDatumFromName(const std::string& name) {
  Types::MatrixDatumPointer matrixDatum = nullptr;
  Types::MatrixDatumMap::iterator iter = mMatrixDatumMap.find(name);

  if (iter != mMatrixDatumMap.end()) {
    matrixDatum = iter->second;
  } else {
    for (unsigned datumIndex = 0; datumIndex < mMatrixDatumMetadata.size(); ++datumIndex) {
      std::string datumName = mMatrixDatumMetadata[datumIndex][Constants::eDatumName];

      if (datumName == name) {
        matrixDatum = new MatrixDatum(datumName);
        mMatrixDatumMap.insert(std::pair<std::string, Types::MatrixDatumPointer>(datumName, matrixDatum));
        break;
      }
    }
  }
  return matrixDatum;
}
