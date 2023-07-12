#include "vertexdata.h"

namespace ng {

Vec3 &Vec3::operator*=(float value) {
  x *= value;
  y *= value;
  z *= value;
  return *this;
}

Vec3 &Vec3::operator+=(Vec3 value) {
  x += value.x;
  y += value.y;
  z += value.z;
  return *this;
}

Vec3 Vec3::operator-(const Vec3 &other) const {
  return {x - other.x, y - other.y, z - other.z};
}

Vec3 Vec3::operator+(const Vec3 &other) const {
  return {x + other.x, y + other.y, z + other.z};
}

Vec3 Vec3::operator*(float value) const {
  return {x * value, y * value, z * value};
}

float Vec3::Dot(const Vec3 &other) const {
  return x * other.x + y * other.y + z * other.z;
}

float Vec3::Absolute() const { return sqrtf(x * x + y * y + z * z); }

float Vec3::Angle(const Vec3 &other) const {
  return acosf(Dot(other) / (Absolute() * other.Absolute()));
}

Vec3 Vec3::Cross(const Vec3 &other) const {
  Vec3 i(1, 0, 0);
  Vec3 j(0, 1, 0);
  Vec3 k(0, 0, 1);
  return i * (y * other.z - z * other.y) - j * (x * other.z - z * other.x) +
         k * (x * other.y - y * other.x);
}

void Vec3::Normalize(float normalize_to) {
  float max = std::fabs(x) > std::fabs(y) ? x : y;
  max = std::fabs(z) > std::fabs(max) ? z : max;
  float aspect = max / normalize_to;
  x /= aspect;
  y /= aspect;
  z /= aspect;
}

VertexData VertexData::operator*(float value) {
  position *= value;
  return *this;
}

VertexData VertexData::operator+(Vec3 value) {
  position += value;
  return *this;
}

const std::vector<VertexData> &ObjectDataModel::GetVertexes() {
  return vertexes_;
}

VertexData ObjectDataModel::GetVertex(std::size_t pos) {
  return vertexes_[pos];
}

const std::vector<unsigned int> &ObjectDataModel::GetIndexes() {
  return indexes_;
}

void ObjectDataModel::SetVertexes(std::vector<VertexData> &&vertexes) {
  vertexes_ = std::move(vertexes);
}

void ObjectDataModel::SetVertex(std::size_t pos, VertexData &&vertex) {
  vertexes_[pos] = std::move(vertex);
}

void ObjectDataModel::SetVertex(const VertexData &v) { vertexes_.push_back(v); }

void ObjectDataModel::SetIndex(unsigned int index) {
  indexes_.push_back(index);
}

void ObjectDataModel::ResizeVertexes(std::size_t size) {
  vertexes_.resize(size);
}

std::size_t ObjectDataModel::SizeVertexes() const { return vertexes_.size(); }

std::size_t ObjectDataModel::SizeIndexes() const { return indexes_.size(); }

int ObjectDataModel::GetFacetsCount() { return facets_count_; }

int ObjectDataModel::GetVertexesCount() { return vertexes_count_; }

int ObjectDataModel::GetEdgesCount() { return edges_count_; }

void ObjectDataModel::SetFacetsCount(int facets) { facets_count_ = facets; }

void ObjectDataModel::SetVertexesCount(int vertexes) {
  vertexes_count_ = vertexes;
}

void ObjectDataModel::SetEdgesCount(int edges) { edges_count_ = edges; }

void ObjectDataModel::Reset() {
  vertexes_.clear();
  indexes_.clear();
  facets_count_ = 0;
  vertexes_count_ = 0;
}

void ObjectDataModel::SetNormals() {
  if (type_ == 4) {
    for (std::size_t i = 0; i < vertexes_.size(); i += 3) {
      VertexData v0 = vertexes_[i];
      VertexData v1 = vertexes_[i + 1];
      VertexData v2 = vertexes_[i + 2];

      Vec3 a = v1.position - v0.position;
      Vec3 b = v2.position - v0.position;
      Vec3 normal(a.Cross(b));
      normal.Normalize(1.f);

      vertexes_[i].normal = normal;
      vertexes_[i + 1].normal = normal;
      vertexes_[i + 2].normal = normal;
    }
  }
}

void ObjectDataModel::SetType(int type) { type_ = type; }

}  // namespace ng
