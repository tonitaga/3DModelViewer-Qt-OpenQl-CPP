#ifndef CPP5_3DVIEWER_V2_1_SRC_CONTROLLER_VIEWERCONTROLLER_H_
#define CPP5_3DVIEWER_V2_1_SRC_CONTROLLER_VIEWERCONTROLLER_H_

#include <string>

#include "../model/facade/viewermodel.h"

namespace ng {
class Scene;

// ----------- ViewerController class -----------
// Main information:
// 1) The class is a controller of ViewerModel class
// 2) The class constructs from ng::Scene class
class ViewerController {
 private:
  Scene *scene_ = nullptr;

 public:
  ViewerController() = default;
  ~ViewerController() = default;

  void SetScene(Scene *scene) { scene_ = scene; }

  void LoadObjModelToScene(const std::string &path);
  void ChangeSceneLightPower(float value);
  void ChangeSceneLightPos(float value, bool x = false, bool y = false,
                           bool z = false);
  void ChangePerspective(bool value);

  void ChangeRotate(float angle, bool x = false, bool y = false,
                    bool z = false);
  void ChangeMove(float move, bool x = false, bool y = false, bool z = false);
  void ChangeScale(float value);
  void ChangeShadowColor(QColor color);
  void ChangeShadow(Shading type);

  void AddTexture(const std::string &path);
  void ResetTexture();

  void RecordImage();
  void RecordGif();
  void RecordGifRotate();

  void SetLineWidth(float width);
  void SetPointSize(float size);
  void SetBackgroundColor(Vec3 color);
  void SetLineColor(Vec3 color);
  void SetPointColor(Vec3 color);
  void SetLineType(int type);
  void SetPointType(int type);
  void SetPrimitiveType(int type);

  int GetVertexesCount();
  int GetFacetsCount();
  int GetEdgesCount();

  Vec3 GetColorBackground();
  Vec3 GetColorLine();
  Vec3 GetColorPoint();

  void Reset();
};
}  // namespace ng

#endif  // !CPP5_3DVIEWER_V2_1_SRC_CONTROLLER_VIEWERCONTROLLER_H_
