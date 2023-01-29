#ifndef FILEREADER
#define FILEREADER

#include "Constants.h"
#include "Types.h"

class FileReader {
 public:
  FileReader();
  ~FileReader();

  void ReadInputFiles(std::string&, std::string&);

 private:
  bool ReadTextFile(const std::string&, bool copyToOutput = true);
  void ClearRawTextData();

  Types::StringMatrix mRawTextData;
};

#endif
