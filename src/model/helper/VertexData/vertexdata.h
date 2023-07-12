#ifndef CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_VERTEXDATA_VERTEXDATA_H_
#define CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_VERTEXDATA_VERTEXDATA_H_

#include <cmath>
#include <vector>

namespace ng {

// ----------- Vec2 struct -----------
// Main information:
// 1) The struct is just a container of two float values
struct Vec2 {
  float x = 0.f;
  float y = 0.f;

  Vec2() = default;
  Vec2(float x, float y) : x(x), y(y) {}
};

// ----------- Vec3 struct -----------
// Main information:
// 1) The struct is a simple math vector realization with their main functions
// 2) The struct is just a container of three float values
struct Vec3 {
  float x = 0.f;
  float y = 0.f;
  float z = 0.f;

  Vec3() = default;
  Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

  Vec3 &operator*=(float value);
  Vec3 &operator+=(Vec3 value);

  Vec3 operator-(const Vec3 &other) const;
  Vec3 operator+(const Vec3 &other) const;
  Vec3 operator*(float value) const;

  [[nodiscard]] float Dot(const Vec3 &other) const;
  [[nodiscard]] float Absolute() const;
  [[nodiscard]] float Angle(const Vec3 &other) const;
  [[nodiscard]] Vec3 Cross(const Vec3 &other) const;

  void Normalize(float normalize_to);
};

// ----------- VertexData struct -----------
// Main information:
// 1) The struct is a have all 3d point values, position, texture, normal
// coordinates
// 2) The struct can be construct from (Vec3, Vec2, Vec3) values
struct VertexData {
  Vec3 position;
  Vec2 tex_coord;
  Vec3 normal;

  VertexData() = default;
  VertexData(Vec3 p, Vec2 t, Vec3 n) : position(p), tex_coord(t), normal(n) {}

  VertexData operator*(float value);
  VertexData operator+(Vec3 value);
};

// ----------- ObjectDataModel struct -----------
// Main information:
// 1) The class is a storage of file with .obj extention
// 2) You can fill this class with data using FileReader class
// 3) Data is storages in std::vector
// 4) Before use function SetVertex(std::size_t, VertexData &&) you must to call
// ResizeVertexes(std::size_t) function!
// 5) Functions SizeVertexes(),SizeIndexes() just return std::vectors size;
class ObjectDataModel {
 private:
  std::vector<VertexData> vertexes_;
  std::vector<unsigned int> indexes_;

  int facets_count_ = 0;
  int vertexes_count_ = 0;
  int edges_count_ = 0;
  int type_ = 0;

 public:
  ObjectDataModel() = default;
  ~ObjectDataModel() = default;

  const std::vector<VertexData> &GetVertexes();
  VertexData GetVertex(std::size_t pos);
  const std::vector<unsigned int> &GetIndexes();
  int GetFacetsCount();
  int GetVertexesCount();
  int GetEdgesCount();

  void SetVertexes(std::vector<VertexData> &&vertexes);
  void SetVertex(std::size_t pos, VertexData &&vertex);
  void SetVertex(const VertexData &v);
  void SetIndex(unsigned int index);
  void SetFacetsCount(int facets);
  void SetVertexesCount(int vertexes);
  void SetEdgesCount(int edges);
  void SetNormals();
  void SetType(int type);

  void ResizeVertexes(std::size_t size);
  std::size_t SizeVertexes() const;
  std::size_t SizeIndexes() const;

  void Reset();
};
}  // namespace ng

#endif  // !CPP5_3DVIEWER_V2_1_SRC_MODEL_HELPER_VERTEXDATA_VERTEXDATA_H_
