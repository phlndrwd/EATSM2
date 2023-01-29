#ifndef FILEREADER
#define FILEREADER

#include <string>
#include <vector>

class FileReader {
 public:
  FileReader();
  ~FileReader();

  void ReadInputFiles(std::string&, std::string&);

 private:
  bool ReadTextFile(const std::string&, bool copyToOutput = true);
  void ClearRawTextData();

  std::vector<std::vector<std::string>> mRawTextData;
};

#endif
