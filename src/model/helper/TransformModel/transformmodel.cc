#include "transformmodel.h"

namespace ng {
void TransformModel::Scale(ObjectDataModel &model, float scale) {
  if (scale >= 1e-6) {
    float current_scale = GetCurrentScale(scale);
    for (std::size_t i = 0; i != model.SizeVertexes(); ++i) {
      model.SetVertex(i, model.GetVertex(i) * current_scale);
    }
    last_scale_ = scale;
  }
}

void TransformModel::Move(ObjectDataModel &model, float move, bool x, bool y,
                          bool z) {
  float current_move = GetCurrentMove(move, x, y, z);
  for (std::size_t i = 0; i != model.SizeVertexes(); ++i) {
    if (x) {
      model.SetVertex(i, model.GetVertex(i) + Vec3(current_move, 0, 0));
    } else if (y) {
      model.SetVertex(i, model.GetVertex(i) + Vec3(0, current_move, 0));
    } else if (z) {
      model.SetVertex(i, model.GetVertex(i) + Vec3(0, 0, current_move));
    }
  }
  SetLastMove(move, x, y, z);
}

void TransformModel::Rotate(ObjectDataModel &model, float angle, bool x, bool y,
                            bool z) {
  float current_angle = GetCurrentRotate(angle, x, y, z);
  for (std::size_t i = 0; i < model.SizeVertexes(); i++) {
    auto vertexData = model.GetVertex(i);
    if (x) {
      vertexData.position =
          Vec3(vertexData.position.x,
               std::cos(current_angle) * vertexData.position.y -
                   std::sin(current_angle) * vertexData.position.z,
               std::sin(current_angle) * vertexData.position.y +
                   std::cos(current_angle) * vertexData.position.z);

    } else if (y) {
      vertexData.position =
          Vec3(std::cos(current_angle) * vertexData.position.x +
                   std::sin(current_angle) * vertexData.position.z,
               vertexData.position.y,
               -std::sin(current_angle) * vertexData.position.x +
                   std::cos(current_angle) * vertexData.position.z);
    } else if (z) {
      vertexData.position =
          Vec3(std::cos(current_angle) * vertexData.position.x -
                   std::sin(current_angle) * vertexData.position.y,
               std::sin(current_angle) * vertexData.position.x +
                   std::cos(current_angle) * vertexData.position.y,
               vertexData.position.z);
    }
    model.SetVertex(i, std::move(vertexData));
  }
  SetLastRotate(angle, x, y, z);
}

/**
 * @param value saving last movement by axis param
 */
void TransformModel::SetLastMove(float value, bool x, bool y, bool z) {
  if (x) {
    last_move_.x = value;
  } else if (y) {
    last_move_.y = value;
  } else {
    last_move_.z = value;
  }
  (void)z;
}

/**
 * @param angle saving last rotation angle param, must be in radians
 */
void TransformModel::SetLastRotate(float angle, bool x, bool y, bool z) {
  angle *= (M_PI / 180.0);
  if (x) {
    last_rotate_.x = angle;
  } else if (y) {
    last_rotate_.y = angle;
  } else {
    last_rotate_.z = angle;
  }
  (void)z;
}

/**
 * @param new_move
 * @return current move by axis
 */
float TransformModel::GetCurrentMove(float new_move, bool x, bool y, bool z) {
  float current_move = 0.f;
  if (x) {
    current_move = new_move - last_move_.x;
  } else if (y) {
    current_move = new_move - last_move_.y;
  } else {
    current_move = new_move - last_move_.z;
  }
  (void)z;
  return current_move;
}

/**
 * @param new_scale
 * @return current scale
 */
float TransformModel::GetCurrentScale(float new_scale) {
  return new_scale / last_scale_;
}

/**
 * @param new_rotate new rotation angle must be in radians
 * @return current rotation angle in degrees
 */
float TransformModel::GetCurrentRotate(float new_rotate, bool x, bool y,
                                       bool z) {
  float current_rotation_degrees = 0.f;
  float new_rotation_degrees = new_rotate * (M_PI / 180.0);

  if (x) {
    current_rotation_degrees = new_rotation_degrees - last_rotate_.x;
  } else if (y) {
    current_rotation_degrees = new_rotation_degrees - last_rotate_.y;
  } else {
    current_rotation_degrees = new_rotation_degrees - last_rotate_.z;
  }
  (void)z;
  return current_rotation_degrees;
}

}  // namespace ng
