#ifndef CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_FILEREADER_FILECHECKER_H_
#define CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_FILEREADER_FILECHECKER_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace ng {

// ----------- ObjectFileType enum -----------
// Main information:
// 1) Enumuration has all types of file with .obj extention
enum ObjectFileType {
  kWithTexture,
  kWithNormal,
  kWithAll,
  kWithError,
  kWithoutTextureNormal
};

// ----------- ViewerModel class -----------
// Main information:
// 1) The class checks the type of file with .obj extention
// 2) The class is based on Singleton pattern, get instance -> call GetChecker()
// static function
// 3) Have only one method, DetectFileType() that returning
// ObjectFileType enumuration
class FileChecker {
 private:
  std::string path_;
  FileChecker() = default;

 public:
  FileChecker(const FileChecker &) = delete;
  FileChecker &operator=(const FileChecker &) = delete;
  ~FileChecker() = default;

  static FileChecker &GetChecker(const std::string &path) {
    static FileChecker checker;
    checker.SetCheckingPath(path);
    return checker;
  }

  ObjectFileType DetectFileType();

 private:
  void SetCheckingPath(std::string path) { path_ = std::move(path); }
  ObjectFileType DetectFileTypeFromParameters(bool, bool, bool, bool);
};
}  // namespace ng
#endif  // !CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_FILEREADER_FILECHECKER_H_
