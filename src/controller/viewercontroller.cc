#include "viewercontroller.h"

namespace ng {
void ViewerController::LoadObjModelToScene(const std::string &path) {
  if (scene_) {
    Reset();
    ViewerModel::GetViewerModel().LoadObjModelToScene(path, scene_);
  }
}

void ViewerController::ChangeSceneLightPower(float value) {
  if (scene_) {
    ViewerModel::GetViewerModel().ChangeSceneLightPower(value, scene_);
  }
}

void ViewerController::ChangeSceneLightPos(float value, bool x, bool y,
                                           bool z) {
  if (scene_) {
    ViewerModel::GetViewerModel().ChangeSceneLightPos(value, scene_, x, y, z);
  }
}

void ViewerController::ChangePerspective(bool value) {
  if (scene_) {
    ViewerModel::GetViewerModel().ChangeScenePerspective(scene_, value);
  }
}

void ViewerController::ChangeRotate(float angle, bool x, bool y, bool z) {
  if (scene_) {
    ViewerModel::GetViewerModel().ChangeSceneRotate(scene_, angle, x, y, z);
  }
}

void ViewerController::ChangeMove(float move, bool x, bool y, bool z) {
  if (scene_) {
    ViewerModel::GetViewerModel().ChangeSceneMove(scene_, move, x, y, z);
  }
}

void ViewerController::ChangeScale(float value) {
  if (scene_) {
    ViewerModel::GetViewerModel().ChangeSceneScale(scene_, value);
  }
}

void ViewerController::ChangeShadowColor(QColor color) {
  if (scene_) {
    ViewerModel::GetViewerModel().ChangeLightColor(scene_, color);
  }
}

void ViewerController::ChangeShadow(Shading type) {
  if (scene_) {
    ViewerModel::GetViewerModel().ChangeShading(scene_, type);
  }
}

void ViewerController::AddTexture(const std::string &path) {
  if (scene_) {
    ViewerModel::GetViewerModel().ChangeTexture(path, scene_);
  }
}

void ViewerController::ResetTexture() {
  if (scene_) {
    ViewerModel::GetViewerModel().ResetTexture(scene_);
  }
}

void ViewerController::RecordImage() {
  if (scene_) {
    ViewerModel::GetViewerModel().MakeImage(scene_);
  }
}

void ViewerController::RecordGif() {
  if (scene_) {
    ViewerModel::GetViewerModel().MakeGif(scene_);
  }
}

void ViewerController::RecordGifRotate() {
  if (scene_) {
    ViewerModel::GetViewerModel().MakeGifRotate(scene_);
  }
}

void ViewerController::SetLineWidth(float width) {
  if (scene_) {
    ViewerModel::GetViewerModel().SetLineWidth(scene_, width);
  }
}

void ViewerController::SetPointSize(float size) {
  if (scene_) {
    ViewerModel::GetViewerModel().SetPointSize(scene_, size);
  }
}

void ViewerController::SetBackgroundColor(Vec3 color) {
  if (scene_) {
    ViewerModel::GetViewerModel().SetBackgroundColor(scene_, color);
  }
}

void ViewerController::SetLineColor(Vec3 color) {
  if (scene_) {
    ViewerModel::GetViewerModel().SetLineColor(scene_, color);
  }
}

void ViewerController::SetPointColor(Vec3 color) {
  if (scene_) {
    ViewerModel::GetViewerModel().SetPointColor(scene_, color);
  }
}

void ViewerController::SetLineType(int type) {
  if (scene_) {
    ViewerModel::GetViewerModel().SetLineType(scene_, type);
  }
}

void ViewerController::SetPointType(int type) {
  if (scene_) {
    ViewerModel::GetViewerModel().SetPointType(scene_, type);
  }
}

void ViewerController::SetPrimitiveType(int type) {
  if (scene_) {
    ViewerModel::GetViewerModel().SetPrimitiveType(scene_, type);
  }
}

int ViewerController::GetVertexesCount() {
  return ViewerModel::GetViewerModel().GetVertexesCount();
}

int ViewerController::GetFacetsCount() {
  return ViewerModel::GetViewerModel().GetFacetsCount();
}

int ViewerController::GetEdgesCount() {
  return ViewerModel::GetViewerModel().GetEdgesCount();
}

Vec3 ViewerController::GetColorBackground() {
  Vec3 color;
  if (scene_) {
    color = ViewerModel::GetViewerModel().GetColorBackground(scene_);
  }
  return color;
}

Vec3 ViewerController::GetColorLine() {
  Vec3 color;
  if (scene_) {
    color = ViewerModel::GetViewerModel().GetColorLine(scene_);
  }
  return color;
}

Vec3 ViewerController::GetColorPoint() {
  Vec3 color;
  if (scene_) {
    color = ViewerModel::GetViewerModel().GetColorPoint(scene_);
  }
  return color;
}

void ViewerController::Reset() { ViewerModel::GetViewerModel().Reset(); }

}  // namespace ng
