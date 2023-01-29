#ifndef FILEREADER
#define FILEREADER

#include <string>
#include <vector>

class FileReader {
 public:
  FileReader();
  ~FileReader();

  void readInputFiles(std::string&, std::string&);

 private:
  bool readTextFile(const std::string&, bool copyToOutput = true);
  void clearRawTextData();

  std::vector<std::vector<std::string>> rawTextData_;
};

#endif
