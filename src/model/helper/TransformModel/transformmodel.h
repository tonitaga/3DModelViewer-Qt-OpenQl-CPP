#ifndef CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_TRANSFORMMODEL_TRANSFORMMODEL_H_
#define CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_TRANSFORMMODEL_TRANSFORMMODEL_H_

#define USE_MATH_DEFINES
#include <cmath>

#include "../VertexData/vertexdata.h"

namespace ng {

// ----------- TransformModel class -----------
// Main information:
// 1) The class is transform ObjectDataModel class using affine transformation
// 2) The class is based on Singleton pattern, get instance -> call
// GetTransform() static function
// 3) The class has 3 main functions:
//        * Scale, Move, Rotate
class TransformModel {
 private:
  TransformModel() : last_move_(), last_scale_(1), last_rotate_(){};

  Vec3 last_move_;
  float last_scale_;
  Vec3 last_rotate_;

 public:
  TransformModel(const TransformModel &) = delete;
  void operator=(const TransformModel &) = delete;

  static TransformModel &GetTransform() {
    static TransformModel trans;
    return trans;
  }

  void Scale(ObjectDataModel &model, float scale);
  void Move(ObjectDataModel &model, float value, bool x = false, bool y = false,
            bool z = false);
  void Rotate(ObjectDataModel &model, float value, bool x = false,
              bool y = false, bool z = false);

 private:
  void SetLastMove(float value, bool x, bool y, bool z);
  void SetLastRotate(float angle, bool x, bool y, bool z);

  float GetCurrentMove(float new_move, bool x, bool y, bool z);
  float GetCurrentScale(float new_scale);
  float GetCurrentRotate(float new_rotate, bool x, bool y, bool z);
};

}  // namespace ng

#endif  // !CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_TRANSFORMMODEL_TRANSFORMMODEL_H_
