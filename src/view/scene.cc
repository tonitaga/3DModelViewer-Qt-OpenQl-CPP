#define GL_SILENCE_DEPRECATION
#include "scene.h"

#include <QDebug>
#include <QMouseEvent>
#include <QOpenGLContext>

namespace ng {

Scene::Scene(QWidget *parent)
    : QOpenGLWidget{parent},
      index_buffer_(QOpenGLBuffer::IndexBuffer),
      texture_(nullptr),
      line_color_(1.0, 0.0, 0.0),
      point_color_(0.0, 1.0, 0.0),
      background_color_(0.5, 0.5, 0.5) {}

Scene::~Scene() {}

void Scene::LoadObjectModel(const std::vector<VertexData> &vertexes,
                            const std::vector<unsigned int> &indexes) {
  vertex_buffer_.destroy();
  index_buffer_.destroy();
  vertexes_.clear();
  indexes_.clear();

  QVector<VertexData> vertexes_to_buffer;
  for (auto vertex : vertexes) {
    vertexes_to_buffer.append(vertex);
    vertexes_.append(vertex.position.x);
    vertexes_.append(vertex.position.y);
    vertexes_.append(vertex.position.z);
  }

  QVector<GLuint> indexes_to_buffer;
  for (auto index : indexes) {
    indexes_to_buffer.append(index);
  }
  indexes_ = indexes_to_buffer;

  vertex_buffer_.create();
  vertex_buffer_.bind();
  vertex_buffer_.allocate(vertexes_to_buffer.constData(),
                          vertexes_to_buffer.size() * sizeof(VertexData));
  vertex_buffer_.release();

  index_buffer_.create();
  index_buffer_.bind();
  index_buffer_.allocate(indexes_to_buffer.constData(),
                         indexes_to_buffer.size() * sizeof(GLuint));
  index_buffer_.release();

  model_is_inited_ = true;
  update();
}

void Scene::LoadTexture(const QString &path) {
  path_ = path;
  texture_ = new QOpenGLTexture(QImage(path_).mirrored());
  texture_->setMinificationFilter(QOpenGLTexture::Nearest);
  texture_->setMagnificationFilter(QOpenGLTexture::Linear);
  texture_->setWrapMode(QOpenGLTexture::Repeat);
  texture_is_inited_ = true;
}

void Scene::initializeGL() {
  glClearColor(background_color_.x, background_color_.y, background_color_.z,
               1.0);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void Scene::resizeGL(int w, int h) {
  float aspect = w / static_cast<float>(h);

  projection_matrix_.setToIdentity();
  projection_matrix_.perspective(45.f, aspect, 0.1, 100.0f);
}

void Scene::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(background_color_.x, background_color_.y, background_color_.z,
               1.0);

  if (model_is_inited_) {
    QMatrix4x4 view_matrix;
    view_matrix.setToIdentity();
    view_matrix.translate(0.0, 0.0, -3.0);

    QMatrix4x4 model_matrix;
    model_matrix.setToIdentity();
    model_matrix.rotate(rotation_);

    if (texture_is_inited_) {
      DrawTextured(view_matrix, model_matrix);
    } else {
      DrawSimple();
    }
  }
}

void Scene::InitShaders() {
  if (!shader_program_.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               ":/vshader.vsh"))
    close();
  if (!shader_program_.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               ":/fshader.fsh"))
    close();
  if (!shader_program_.link()) close();
  shaders_is_linked_ = true;
}

void Scene::DrawTextured(QMatrix4x4 &view_matrix, QMatrix4x4 &model_matrix) {
  if (GetPerspective()) {
    view_matrix.frustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1.0);
    view_matrix.translate(0, 0, -0.5 * 3.0);
  }
  if (!shaders_is_linked_) {
    InitShaders();
  }
  shader_program_.bind();
  texture_->bind(0);

  SetUniformValuesToFShader(view_matrix, model_matrix);

  vertex_buffer_.bind();

  int offset = 0;
  int vertex_location = shader_program_.attributeLocation("a_position");
  shader_program_.enableAttributeArray(vertex_location);
  shader_program_.setAttributeBuffer(vertex_location, GL_FLOAT, offset, 3,
                                     sizeof(VertexData));

  offset += sizeof(QVector3D);
  int texture_location = shader_program_.attributeLocation("a_texcoord");
  shader_program_.enableAttributeArray(texture_location);
  shader_program_.setAttributeBuffer(texture_location, GL_FLOAT, offset, 2,
                                     sizeof(VertexData));

  offset += sizeof(QVector2D);
  int normal_location = shader_program_.attributeLocation("a_normal");
  shader_program_.enableAttributeArray(normal_location);
  shader_program_.setAttributeBuffer(normal_location, GL_FLOAT, offset, 3,
                                     sizeof(VertexData));

  index_buffer_.bind();
  glDrawElements(GL_TRIANGLES, index_buffer_.size(), GL_UNSIGNED_INT, nullptr);

  shader_program_.release();
  vertex_buffer_.release();
  index_buffer_.release();
  texture_->release();
}

void Scene::DrawSimple() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (GetPerspective()) {
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 10.0);
    glTranslatef(0, 0, -0.5 * 3.0);
  } else {
    glOrtho(-1, 1, -1, 1, 0.1, 10);
    glTranslatef(0, 0, -3.0);
  }

  glVertexPointer(3, GL_FLOAT, 0, vertexes_.constData());
  glEnableClientState(GL_VERTEX_ARRAY);
  if (point_type_ == 1) {
    glEnable(GL_POINT_SMOOTH);
  }

  if (point_type_ != 2) {
    glPointSize(point_size_);
    glColor3f(point_color_.x, point_color_.y, point_color_.z);
    glDrawElements(GL_POINTS, indexes_.size(), GL_UNSIGNED_INT,
                   indexes_.constData());
  }
  glEnable(GL_LINE_STIPPLE);
  glLineWidth(line_width_);
  if (line_type_ == 1) {
    glLineStipple(2, 0X11FF);
  } else if (line_type_ == 2) {
    glLineStipple(2, 0X00FF);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  glColor3f(line_color_.x, line_color_.y, line_color_.z);
  if (primitive_type_ == 0) {
    glDrawElements(GL_TRIANGLES, indexes_.size(), GL_UNSIGNED_INT,
                   indexes_.constData());
  } else {
    glDrawElements(GL_LINES, indexes_.size(), GL_UNSIGNED_INT,
                   indexes_.constData());
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}

void Scene::SetUniformValuesToFShader(QMatrix4x4 &view_matrix,
                                      QMatrix4x4 &model_matrix) {
  shader_program_.setUniformValue("u_projection_matrix", projection_matrix_);
  shader_program_.setUniformValue("u_view_matrix", view_matrix);
  shader_program_.setUniformValue("u_model_matrix", model_matrix);
  shader_program_.setUniformValue("u_light_position", light_position_);
  shader_program_.setUniformValue("u_light_power", light_power_);
  shader_program_.setUniformValue("u_light_color", light_color_);
  shader_program_.setUniformValue("typeShading", typeShading);
  shader_program_.setUniformValue("u_texture", 0);
}

void Scene::SetLightPower(float light_power) {
  light_power_ = light_power;
  update();
}

void Scene::SetLightPosition(float value, bool x, bool y, bool z) {
  if (x) {
    light_position_.setX(value);
  } else if (y) {
    light_position_.setY(value);
  } else if (z) {
    light_position_.setZ(value);
  }
  update();
}

void Scene::SetPerspective(bool value) { projection_is_perspective_ = value; }

void Scene::SetTextureFlag() {
  if (!path_.isEmpty()) {
    texture_is_inited_ = !texture_is_inited_;
    update();
  }
}

void Scene::SetLightColor(QColor ribs) {
  light_color_ = QVector3D(ribs.redF(), ribs.greenF(), ribs.blueF());
}

void Scene::SetShading(Shading type) { typeShading = type; }

bool Scene::GetPerspective() { return projection_is_perspective_; }

void Scene::SetLineWidth(float width) { line_width_ = width; }

float Scene::GetLineWidth() const { return line_width_; }

void Scene::SetPointSize(float size) { point_size_ = size; }

float Scene::GetPointSize() const { return point_size_; }

void Scene::SetLineColor(Vec3 color) { line_color_ = color; }

Vec3 Scene::GetLineColor() const { return line_color_; }

void Scene::SetPointColor(Vec3 color) { point_color_ = color; }

Vec3 Scene::GetPointColor() const { return point_color_; }

void Scene::SetBackgroundColor(Vec3 color) { background_color_ = color; }

Vec3 Scene::GetBackgroundColor() const { return background_color_; }

void Scene::SetLineType(int type) { line_type_ = type; }

int Scene::GetLineType() const { return line_type_; }

void Scene::SetPointType(int type) { point_type_ = type; }

void Scene::SetPrimitiveType(int type) { primitive_type_ = type; }

int Scene::GetPointType() const { return point_type_; }

void Scene::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    mouse_position_ = QVector2D(event->position());
  }
  event->accept();
}

void Scene::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    QVector2D difference = QVector2D(event->position()) - mouse_position_;
    mouse_position_ = QVector2D(event->position());

    float angle = difference.length() / 5.f;
    QVector3D axis = QVector3D(difference.y(), difference.x(), 0.0);
    rotation_ = QQuaternion::fromAxisAndAngle(axis, angle) * rotation_;
    update();
  }
}

}  // namespace ng
