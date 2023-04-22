#ifndef FILEWRITER
#define FILEWRITER

#include <string>

class Environment;

class FileWriter {
 public:
  FileWriter();

  void writeOutputData(Environment&);

 private:
  void initialiseOutputDirectory();
  bool writeInputFiles();

  bool writeVectorDatums();
  bool writeMatrixDatums();
  bool writeStateFile(Environment&);

  std::string outputPath_;
  std::string dataSetDirectoryName_;
};

#endif
