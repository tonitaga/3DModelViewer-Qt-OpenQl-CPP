#include "viewermodel.h"

#include "../../view/scene.h"
#include "../helper/FileReader/filereader.h"
#include "../helper/Normalize/normalizer.h"
#include "../helper/ObjectRecorder/objectrecorder.h"
#include "../helper/TransformModel/transformmodel.h"

namespace ng {
void ViewerModel::LoadObjModelToScene(const std::string &path, Scene *scene) {
  int current_index = GetModelIndexFromHistory(path);
  bool is_loadable = false;
  object_data_.Reset();
  if (current_index == -1) {
    auto result = FileReader::GetReader().Read(path);
    object_data_ = result.first;
    Normalizer::GetNormalizer().Normalize(object_data_, 1.0f);
    AddModelToHistory(path);
    is_loadable = result.second;
  } else {
    object_data_ = object_data_history_[current_index].second;
  }

  if (is_loadable) {
    scene->LoadObjectModel(object_data_.GetVertexes(),
                           object_data_.GetIndexes());
  }
}

void ViewerModel::ChangeSceneLightPower(float value, Scene *scene) {
  scene->SetLightPower(value / 25.f);
}

void ViewerModel::ChangeSceneLightPos(float value, Scene *scene, bool x, bool y,
                                      bool z) {
  scene->SetLightPosition(value, x, y, z);
}

void ViewerModel::ChangeScenePerspective(Scene *scene, bool value) {
  scene->SetPerspective(value);
  scene->update();
}

void ViewerModel::ChangeSceneRotate(Scene *scene, float angle, bool x, bool y,
                                    bool z) {
  if (x) {
    TransformModel::GetTransform().Rotate(object_data_, angle, x);
  } else if (y) {
    TransformModel::GetTransform().Rotate(object_data_, angle, x, y);
  } else if (z) {
    TransformModel::GetTransform().Rotate(object_data_, angle, x, y, z);
  }
  scene->LoadObjectModel(object_data_.GetVertexes(), object_data_.GetIndexes());
}

void ViewerModel::ChangeSceneScale(Scene *scene, float scale) {
  if (scale >= 1e-6) {
    TransformModel::GetTransform().Scale(object_data_, scale);
    scene->LoadObjectModel(object_data_.GetVertexes(),
                           object_data_.GetIndexes());
  }
}

void ViewerModel::ChangeSceneMove(Scene *scene, float move, bool x, bool y,
                                  bool z) {
  if (x) {
    TransformModel::GetTransform().Move(object_data_, move / 75, x);
  } else if (y) {
    TransformModel::GetTransform().Move(object_data_, move / 75, x, y);
  } else if (z) {
    TransformModel::GetTransform().Move(object_data_, move / 75, x, y, z);
  }
  scene->LoadObjectModel(object_data_.GetVertexes(), object_data_.GetIndexes());
}

void ViewerModel::ChangeLightColor(Scene *scene, QColor ribs) {
  scene->SetLightColor(ribs);
}

void ViewerModel::ChangeShading(Scene *scene, Shading type) {
  scene->SetShading(type);
  scene->update();
}

void ViewerModel::ChangeTexture(const std::string &path, Scene *scene) {
  auto tmp = QString::fromStdString(path);
  scene->LoadTexture(tmp);
}

void ViewerModel::ResetTexture(Scene *scene) { scene->SetTextureFlag(); }

void ViewerModel::MakeImage(Scene *scene) {
  recorder_.SetRecorder(new RecorderImage);
  recorder_.RecordObject(scene, kImage);
}

void ViewerModel::MakeGif(Scene *scene) {
  recorder_.SetRecorder(new RecorderGif(object_data_));
  recorder_.RecordObject(scene, kGif);
}

void ViewerModel::MakeGifRotate(Scene *scene) {
  recorder_.SetRecorder(new RecorderGif(object_data_));
  recorder_.RecordObject(scene, kGifRotation);
}

int ViewerModel::GetVertexesCount() { return object_data_.GetVertexesCount(); }

int ViewerModel::GetFacetsCount() { return object_data_.GetFacetsCount(); }

int ViewerModel::GetEdgesCount() { return object_data_.GetEdgesCount(); }

void ViewerModel::SetLineWidth(Scene *scene, float width) {
  scene->SetLineWidth(width);
  scene->update();
}

void ViewerModel::SetPointSize(Scene *scene, float size) {
  scene->SetPointSize(size);
  scene->update();
}

void ViewerModel::SetBackgroundColor(Scene *scene, Vec3 color) {
  scene->SetBackgroundColor(color);
  scene->update();
}

void ViewerModel::SetLineColor(Scene *scene, Vec3 color) {
  scene->SetLineColor(color);
  scene->update();
}

void ViewerModel::SetPointColor(Scene *scene, Vec3 color) {
  scene->SetPointColor(color);
  scene->update();
}

void ViewerModel::SetLineType(Scene *scene, int type) {
  scene->SetLineType(type);
  scene->update();
}

void ViewerModel::SetPointType(Scene *scene, int type) {
  scene->SetPointType(type);
  scene->update();
}

void ViewerModel::SetPrimitiveType(Scene *scene, int type) {
  scene->SetPrimitiveType(type);
  scene->update();
}

Vec3 ViewerModel::GetColorBackground(Scene *scene) {
  return scene->GetBackgroundColor();
}

Vec3 ViewerModel::GetColorLine(Scene *scene) { return scene->GetLineColor(); }

Vec3 ViewerModel::GetColorPoint(Scene *scene) { return scene->GetPointColor(); }

void ViewerModel::AddModelToHistory(std::string path) {
  if (GetModelIndexFromHistory(path) == -1) {
    object_data_history_.push_back(std::make_pair(
        path.substr(path.find_last_of("/\\") + 1), object_data_));
  }
}

int ViewerModel::GetModelIndexFromHistory(std::string path) {
  std::string filename = path.substr(path.find_last_of("/\\") + 1);
  for (std::size_t i = 0; i != object_data_history_.size(); ++i)
    if (object_data_history_[i].first == filename) return i;
  return -1;
}

}  // namespace ng
