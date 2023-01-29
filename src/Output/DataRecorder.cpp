#include "DataRecorder.h"

#include "Constants.h"
#include "MatrixDatum.h"
#include "Strings.h"
#include "VectorDatum.h"

DataRecorder* DataRecorder::mThis = nullptr;

DataRecorder* DataRecorder::Get() {
  if (mThis == nullptr) mThis = new DataRecorder();

  return mThis;
}

DataRecorder::DataRecorder() {}

DataRecorder::~DataRecorder() {
  for (std::map<std::string, VectorDatum*>::iterator iter = mVectorDatumMap.begin(); iter != mVectorDatumMap.end(); ++iter) {
    delete iter->second;
  }
  for (std::map<std::string, MatrixDatum*>::iterator iter = mMatrixDatumMap.begin(); iter != mMatrixDatumMap.end(); ++iter) {
    delete iter->second;
  }
  if (mThis != nullptr) delete mThis;
}

bool DataRecorder::Initialise(const std::vector<std::vector<std::string>>& rawOutputParameterData) {
  if (rawOutputParameterData.size() > 0) {
    for (unsigned rowIndex = 0; rowIndex < rawOutputParameterData.size(); ++rowIndex) {
      std::string name = Strings::RemoveWhiteSpace(rawOutputParameterData[rowIndex][Constants::eDatumName]);
      std::string type =
          Strings::RemoveWhiteSpace(Strings::ToLowercase(rawOutputParameterData[rowIndex][Constants::eDatumType]));

      std::vector<std::string> datumMetadata;
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
  MatrixDatum* matrixDatum = GetMatrixDatumFromName(name);

  if (matrixDatum != nullptr) matrixDatum->SetGroupSize(size);
}

void DataRecorder::AddDataTo(const std::string& name, const float& data) {
  VectorDatum* vectorDatum = GetVectorDatumFromName(name);

  if (vectorDatum != nullptr) vectorDatum->AddData(data);
}

void DataRecorder::AddDataTo(const std::string& name, const std::vector<float> data) {
  MatrixDatum* matrixDatum = GetMatrixDatumFromName(name);

  if (matrixDatum != nullptr) matrixDatum->AddData(data);
}

void DataRecorder::AddDataTo(const std::string& name, const unsigned& index, const float& data) {
  MatrixDatum* matrixDatum = GetMatrixDatumFromName(name);

  if (matrixDatum != nullptr) matrixDatum->AddDataAtIndex(index, data);
}

void DataRecorder::SetVectorDataOn(const std::string& name, const std::vector<float> data) {
  VectorDatum* vectorDatum = GetVectorDatumFromName(name);

  if (vectorDatum != nullptr) vectorDatum->SetData(data);
}

void DataRecorder::AddInputFilePath(const std::string& inputFilePath) { mInputFilePaths.push_back(inputFilePath); }

std::map<std::string, VectorDatum*> DataRecorder::GetVectorDatumMap() const { return mVectorDatumMap; }

std::map<std::string, MatrixDatum*> DataRecorder::GetMatrixDatumMap() const { return mMatrixDatumMap; }

std::vector<std::string> DataRecorder::GetInputFilePaths() const { return mInputFilePaths; }

VectorDatum* DataRecorder::GetVectorDatumFromName(const std::string& name) {
  VectorDatum* vectorDatum = nullptr;
  std::map<std::string, VectorDatum*>::iterator iter = mVectorDatumMap.find(name);

  if (iter != mVectorDatumMap.end()) {
    vectorDatum = iter->second;
  } else {
    for (unsigned datumIndex = 0; datumIndex < mVectorDatumMetadata.size(); ++datumIndex) {
      std::string datumName = mVectorDatumMetadata[datumIndex][Constants::eDatumName];

      if (datumName == name) {
        vectorDatum = new VectorDatum(datumName);
        mVectorDatumMap.insert(std::pair<std::string, VectorDatum*>(datumName, vectorDatum));
        break;
      }
    }
  }
  return vectorDatum;
}

MatrixDatum* DataRecorder::GetMatrixDatumFromName(const std::string& name) {
  MatrixDatum* matrixDatum = nullptr;
  std::map<std::string, MatrixDatum*>::iterator iter = mMatrixDatumMap.find(name);

  if (iter != mMatrixDatumMap.end()) {
    matrixDatum = iter->second;
  } else {
    for (unsigned datumIndex = 0; datumIndex < mMatrixDatumMetadata.size(); ++datumIndex) {
      std::string datumName = mMatrixDatumMetadata[datumIndex][Constants::eDatumName];

      if (datumName == name) {
        matrixDatum = new MatrixDatum(datumName);
        mMatrixDatumMap.insert(std::pair<std::string, MatrixDatum*>(datumName, matrixDatum));
        break;
      }
    }
  }
  return matrixDatum;
}
