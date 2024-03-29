/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "DataRecorder.h"

#include "Constants.h"
#include "MatrixDatum.h"
#include "Strings.h"
#include "Enums.h"
#include "VectorDatum.h"

DataRecorder* DataRecorder::this_ = nullptr;

DataRecorder* DataRecorder::get() {
  if (this_ == nullptr) this_ = new DataRecorder();

  return this_;
}

DataRecorder::DataRecorder() {}

DataRecorder::~DataRecorder() {
  for (std::map<std::string, VectorDatum*>::iterator iter = vectorDatumMap_.begin(); iter != vectorDatumMap_.end(); ++iter) {
    delete iter->second;
  }
  for (std::map<std::string, MatrixDatum*>::iterator iter = matrixDatumMap_.begin(); iter != matrixDatumMap_.end(); ++iter) {
    delete iter->second;
  }
  if (this_ != nullptr) delete this_;
}

bool DataRecorder::initialise(const std::vector<std::vector<std::string>>& rawOutputParameterData) {
  if (rawOutputParameterData.size() > 0) {
    for (unsigned rowIndex = 0; rowIndex < rawOutputParameterData.size(); ++rowIndex) {
      std::string name = Strings::removeWhiteSpace(rawOutputParameterData[rowIndex][enums::eDatumName]);
      std::string type =
          Strings::removeWhiteSpace(Strings::toLowercase(rawOutputParameterData[rowIndex][enums::eDatumType]));

      std::vector<std::string> datumMetadata;
      datumMetadata.push_back(name);
      datumMetadata.push_back(type);

      if (type == constants::kVectorDatumTypeName) {
        vectorDatumMetadata_.push_back(datumMetadata);
      } else if (type == constants::kMatrixDatumTypeName) {
        matrixDatumMetadata_.push_back(datumMetadata);
      }
    }
    return true;
  } else {
    return false;
  }
}

void DataRecorder::initialiseMatrix(const std::string& name, const unsigned& size) {
  MatrixDatum* matrixDatum = getMatrixDatumFromName(name);

  if (matrixDatum != nullptr) matrixDatum->setGroupSize(size);
}

void DataRecorder::addDataTo(const std::string& name, const float& data) {
  VectorDatum* vectorDatum = getVectorDatumFromName(name);

  if (vectorDatum != nullptr) vectorDatum->addData(data);
}

void DataRecorder::addDataTo(const std::string& name, const std::vector<float> data) {
  MatrixDatum* matrixDatum = getMatrixDatumFromName(name);

  if (matrixDatum != nullptr) matrixDatum->addData(data);
}

void DataRecorder::addDataTo(const std::string& name, const unsigned& index, const float& data) {
  MatrixDatum* matrixDatum = getMatrixDatumFromName(name);

  if (matrixDatum != nullptr) matrixDatum->addDataAtIndex(index, data);
}

void DataRecorder::setVectorDataOn(const std::string& name, const std::vector<float> data) {
  VectorDatum* vectorDatum = getVectorDatumFromName(name);

  if (vectorDatum != nullptr) vectorDatum->setData(data);
}

void DataRecorder::addInputFilePath(const std::string& inputFilePath) { inputFilePaths_.push_back(inputFilePath); }

std::map<std::string, VectorDatum*> DataRecorder::getVectorDatumMap() const { return vectorDatumMap_; }

std::map<std::string, MatrixDatum*> DataRecorder::getMatrixDatumMap() const { return matrixDatumMap_; }

std::vector<std::string> DataRecorder::getInputFilePaths() const { return inputFilePaths_; }

VectorDatum* DataRecorder::getVectorDatumFromName(const std::string& name) {
  VectorDatum* vectorDatum = nullptr;
  std::map<std::string, VectorDatum*>::iterator iter = vectorDatumMap_.find(name);

  if (iter != vectorDatumMap_.end()) {
    vectorDatum = iter->second;
  } else {
    for (unsigned datumIndex = 0; datumIndex < vectorDatumMetadata_.size(); ++datumIndex) {
      std::string datumName = vectorDatumMetadata_[datumIndex][enums::eDatumName];

      if (datumName == name) {
        vectorDatum = new VectorDatum(datumName);
        vectorDatumMap_.insert(std::pair<std::string, VectorDatum*>(datumName, vectorDatum));
        break;
      }
    }
  }
  return vectorDatum;
}

MatrixDatum* DataRecorder::getMatrixDatumFromName(const std::string& name) {
  MatrixDatum* matrixDatum = nullptr;
  std::map<std::string, MatrixDatum*>::iterator iter = matrixDatumMap_.find(name);

  if (iter != matrixDatumMap_.end()) {
    matrixDatum = iter->second;
  } else {
    for (unsigned datumIndex = 0; datumIndex < matrixDatumMetadata_.size(); ++datumIndex) {
      std::string datumName = matrixDatumMetadata_[datumIndex][enums::eDatumName];

      if (datumName == name) {
        matrixDatum = new MatrixDatum(datumName);
        matrixDatumMap_.insert(std::pair<std::string, MatrixDatum*>(datumName, matrixDatum));
        break;
      }
    }
  }
  return matrixDatum;
}
