#ifndef CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_NORMALIZE_NORMALIZER_H_
#define CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_NORMALIZE_NORMALIZER_H_

#include "../VertexData/vertexdata.h"

namespace ng {

// ----------- FileReader class -----------
// Main information:
// 1) The class normalize ObjectDataModel class data to some float value
// 2) The class is based on Singleton pattern, get instance -> call
// GetNormalizer() static function
class Normalizer {
 private:
  Normalizer() = default;

 public:
  Normalizer(const Normalizer &) = delete;
  Normalizer &operator=(const Normalizer &) = delete;

  static Normalizer &GetNormalizer();

  void Normalize(ObjectDataModel &data, float normalize_to);
};

}  // namespace ng

#endif  // !CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_NORMALIZE_NORMALIZER_H_
