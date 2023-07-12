#ifndef CPP5_3DVIEWER_V2_1_SRC_VIEW_SCENE_H
#define CPP5_3DVIEWER_V2_1_SRC_VIEW_SCENE_H

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

#include "../model/helper/VertexData/vertexdata.h"

namespace ng {

enum Shading { kFlatShading = 1, kGuroShading, kPhongShading };

class Scene : public QOpenGLWidget {
  Q_OBJECT
 public:
  explicit Scene(QWidget *parent = nullptr);
  ~Scene();

  void LoadObjectModel(const std::vector<VertexData> &vertexes,
                       const std::vector<unsigned int> &indexes);
  void LoadTexture(const QString &path);

  void SetLightPower(float ligth_power);
  void SetLightPosition(float value, bool x = 0, bool y = 0, bool z = 0);
  void SetPerspective(bool value);
  void SetTextureFlag();
  void SetLightColor(QColor ribs);
  void SetShading(Shading type);
  void SetLineWidth(float width);
  void SetPointSize(float size);
  void SetLineColor(Vec3 color);
  void SetPointColor(Vec3 color);
  void SetBackgroundColor(Vec3 color);
  void SetLineType(int type);
  void SetPointType(int type);
  void SetPrimitiveType(int type);

  bool GetPerspective();
  float GetLineWidth() const;
  float GetPointSize() const;
  Vec3 GetLineColor() const;
  Vec3 GetPointColor() const;
  Vec3 GetBackgroundColor() const;
  int GetLineType() const;
  int GetPointType() const;

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  void InitShaders();
  void LoadObj();

 private:
  QVector2D mouse_position_;
  QQuaternion rotation_;

  QOpenGLShaderProgram shader_program_;
  QMatrix4x4 projection_matrix_;

  QOpenGLBuffer vertex_buffer_;
  QOpenGLBuffer index_buffer_;
  QOpenGLTexture *texture_;

  QVector4D light_position_ = QVector4D(0.0, 0.0, 0.0, 1.0);
  QVector3D light_color_ = QVector3D(0.0, 0.0, 1.0);

  bool model_is_inited_ = false;
  bool projection_is_perspective_ = false;
  bool texture_is_inited_ = false;
  bool shaders_is_linked_ = false;

  QVector<float> vertexes_;
  QVector<GLuint> indexes_;
  Shading typeShading = kFlatShading;
  QString file_name_ = nullptr;
  float light_power_ = 5.0f;
  QString path_;

  /* Model settings */
  float line_width_ = 1.0f;
  float point_size_ = 2.0f;
  Vec3 line_color_;
  Vec3 point_color_;
  Vec3 background_color_;
  int line_type_ = 0;
  int point_type_ = 0;
  int primitive_type_ = 0;

  void DrawTextured(QMatrix4x4 &view_matrix, QMatrix4x4 &model_matrix);
  void DrawSimple();
  void SetUniformValuesToFShader(QMatrix4x4 &view_matrix,
                                 QMatrix4x4 &model_matrix);
};
}  // namespace ng

#endif  // !CPP5_3DVIEWER_V2_1_SRC_VIEW_SCENE_H
