#ifndef CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_FILEREADER_FILEREADER_H_
#define CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_FILEREADER_FILEREADER_H_

#include "../VertexData/vertexdata.h"
#include "filechecker.h"

namespace ng {

// ----------- FileReader class -----------
// Main information:
// 1) The class read file with .obj file extention and return ObjectDataModel
// class instance
// 2) The class is based on Singleton pattern, get instance ->
// call GetReader() static function
class FileReader {
 private:
  FileReader() = default;

 public:
  FileReader(const FileReader &) = delete;
  FileReader &operator=(const FileReader &) = delete;

  static FileReader &GetReader();

  std::pair<ObjectDataModel, bool> Read(const std::string &path);

 private:
  struct with_all_ {};
  struct with_normal_ {};
  struct without_texture_normal_ {};

  std::pair<ObjectDataModel, bool> Read(const std::string &path, with_all_);
  std::pair<ObjectDataModel, bool> Read(const std::string &path, with_normal_);
  std::pair<ObjectDataModel, bool> Read(const std::string &path,
                                        without_texture_normal_);

  ObjectFileType type_{};
};
}  // namespace ng

#endif  // !CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_FILEREADER_FILEREADER_H_
