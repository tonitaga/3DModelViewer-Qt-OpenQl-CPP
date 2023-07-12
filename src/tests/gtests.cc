#include <filesystem>

#include "../model/helper/FileReader/filereader.h"
#include "../model/helper/Normalize/normalizer.h"
#include "../model/helper/TransformModel/transformmodel.h"
#include "../model/helper/VertexData/vertexdata.h"
#include "gtest/gtest.h"

TEST(Test, MoveX) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/cube2.obj"));
  ng::TransformModel::GetTransform().Move(result.first, 2, true, false, false);
  const auto &vertexes = result.first.GetVertexes();
  std::vector<std::vector<float>> correct_vertexes{
      {2.f, 0.f, 0.f}, {4.f, 2.f, 0.f}, {4.f, 0.f, 0.f}, {2.f, 0.f, 0.f},
      {2.f, 2.f, 0.f}, {4.f, 2.f, 0.f}, {2.f, 0.f, 0.f}, {2.f, 2.f, 2.f},
      {2.f, 2.f, 0.f}, {2.f, 0.f, 0.f}, {2.f, 0.f, 2.f}, {2.f, 2.f, 2.f},
  };
  for (std::size_t i = 0; i < correct_vertexes.size(); i++) {
    ASSERT_FLOAT_EQ(vertexes[i].position.x, correct_vertexes[i][0]);
  }
}

TEST(Test, MoveY) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/cube2.obj"));
  ng::TransformModel::GetTransform().Move(result.first, 2, false, true, false);
  const auto &vertexes = result.first.GetVertexes();
  std::vector<std::vector<float>> correct_vertexes{
      {0.f, 2.f, 0.f}, {2.f, 4.f, 0.f}, {2.f, 2.f, 0.f}, {0.f, 2.f, 0.f},
      {0.f, 4.f, 0.f}, {2.f, 4.f, 0.f}, {0.f, 2.f, 0.f}, {0.f, 4.f, 2.f},
      {0.f, 4.f, 0.f}, {0.f, 2.f, 0.f}, {0.f, 2.f, 2.f}, {0.f, 4.f, 2.f},
  };
  for (std::size_t i = 0; i < correct_vertexes.size(); i++) {
    ASSERT_FLOAT_EQ(vertexes[i].position.y, correct_vertexes[i][1]);
  }
}

TEST(Test, MoveZ) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/cube2.obj"));
  ng::TransformModel::GetTransform().Move(result.first, 2, false, false, true);
  const auto &vertexes = result.first.GetVertexes();
  std::vector<std::vector<float>> correct_vertexes{
      {0.f, 0.f, 2.f}, {2.f, 2.f, 2.f}, {2.f, 0.f, 2.f}, {0.f, 0.f, 2.f},
      {0.f, 2.f, 2.f}, {2.f, 2.f, 2.f}, {0.f, 0.f, 2.f}, {0.f, 2.f, 4.f},
      {0.f, 2.f, 2.f}, {0.f, 0.f, 2.f}, {0.f, 0.f, 4.f}, {0.f, 2.f, 4.f},
  };
  for (std::size_t i = 0; i < correct_vertexes.size(); i++) {
    ASSERT_FLOAT_EQ(vertexes[i].position.z, correct_vertexes[i][2]);
  }
}

TEST(Test, Scale) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/cube2.obj"));
  ng::TransformModel::GetTransform().Scale(result.first, 2);
  const auto &vertexes = result.first.GetVertexes();
  std::vector<std::vector<float>> correct_vertexes{
      {0.f, 0.f, 0.f}, {2.f, 2.f, 0.f}, {2.f, 0.f, 0.f}, {0.f, 0.f, 0.f},
      {0.f, 2.f, 0.f}, {2.f, 2.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 2.f, 2.f},
      {0.f, 2.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 2.f}, {0.f, 2.f, 2.f},
  };

  for (std::size_t i = 0; i != correct_vertexes.size(); ++i) {
    for (std::size_t j = 0; j != correct_vertexes[i].size(); ++j) {
      correct_vertexes[i][j] *= 2;
    }
  }
  for (std::size_t i = 0; i < correct_vertexes.size(); i++) {
    ASSERT_FLOAT_EQ(vertexes[i].position.x, correct_vertexes[i][0]);
    ASSERT_FLOAT_EQ(vertexes[i].position.y, correct_vertexes[i][1]);
    ASSERT_FLOAT_EQ(vertexes[i].position.z, correct_vertexes[i][2]);
  }
}

TEST(Test, RotateX) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/FinalBaseMesh.obj"));
  const auto &vertexes = result.first.GetVertexes();
  ng::TransformModel::GetTransform().Rotate(result.first, 0, true, false,
                                             false);
  const auto &vertexes_correct = result.first.GetVertexes();

  for (std::size_t i = 0; i != vertexes.size(); ++i) {
    ASSERT_FLOAT_EQ(vertexes[i].position.x, vertexes_correct[i].position.x);
    ASSERT_FLOAT_EQ(vertexes[i].position.y, vertexes_correct[i].position.y);
    ASSERT_FLOAT_EQ(vertexes[i].position.z, vertexes_correct[i].position.z);
  }
}

TEST(Test, RotateY) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/FinalBaseMesh.obj"));
  const auto &vertexes = result.first.GetVertexes();
  ng::TransformModel::GetTransform().Rotate(result.first, 0, false, true,
                                             false);
  const auto &vertexes_correct = result.first.GetVertexes();

  for (std::size_t i = 0; i != vertexes.size(); ++i) {
    ASSERT_FLOAT_EQ(vertexes[i].position.x, vertexes_correct[i].position.x);
    ASSERT_FLOAT_EQ(vertexes[i].position.y, vertexes_correct[i].position.y);
    ASSERT_FLOAT_EQ(vertexes[i].position.z, vertexes_correct[i].position.z);
  }
}

TEST(Test, RotateZ) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/FinalBaseMesh.obj"));
  const auto &vertexes = result.first.GetVertexes();
  ng::TransformModel::GetTransform().Rotate(result.first, 0, false, false,
                                             true);
  const auto &vertexes_correct = result.first.GetVertexes();

  for (std::size_t i = 0; i != vertexes.size(); ++i) {
    ASSERT_FLOAT_EQ(vertexes[i].position.x, vertexes_correct[i].position.x);
    ASSERT_FLOAT_EQ(vertexes[i].position.y, vertexes_correct[i].position.y);
    ASSERT_FLOAT_EQ(vertexes[i].position.z, vertexes_correct[i].position.z);
  }
}

TEST(Test, ParseFile1) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/cube.obj"));
  ASSERT_DOUBLE_EQ(result.first.GetVertexesCount(), 8);
  ASSERT_DOUBLE_EQ(result.first.GetFacetsCount(), 12);
}

TEST(Test, ParseFile2) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/cube2.obj"));
  ASSERT_DOUBLE_EQ(result.first.GetVertexesCount(), 9);
  ASSERT_DOUBLE_EQ(result.first.GetFacetsCount(), 10);
}

TEST(Test, ParseFile3) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/FinalBaseMesh.obj"));
  ASSERT_DOUBLE_EQ(result.first.GetVertexesCount(), 24461);
  ASSERT_DOUBLE_EQ(result.first.GetFacetsCount(), 24459);
}

TEST(Test, ParseFile4) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/awp.obj"));
  ASSERT_DOUBLE_EQ(result.first.GetVertexesCount(), 11577);
  ASSERT_DOUBLE_EQ(result.first.GetFacetsCount(), 21916);
}

TEST(Test, Vec3OperatorPlusMul) {
  ng::Vec3 vec1(2, 2, 2);
  ng::Vec3 vec2;

  vec1 *= 3;
  vec2 += vec1;

  ASSERT_FLOAT_EQ(vec1.x, 6.f);
  ASSERT_FLOAT_EQ(vec1.y, 6.f);
  ASSERT_FLOAT_EQ(vec1.z, 6.f);
  ASSERT_FLOAT_EQ(vec2.x, 6.f);
  ASSERT_FLOAT_EQ(vec2.y, 6.f);
  ASSERT_FLOAT_EQ(vec2.z, 6.f);
}

TEST(Test, Vec3OperatorPlusMinusMul) {
  ng::Vec3 vec1(2, 2, 2);
  ng::Vec3 vec2;

  vec2 = vec2 + vec1;
  ASSERT_FLOAT_EQ(vec2.x, 2.f);
  ASSERT_FLOAT_EQ(vec2.y, 2.f);
  ASSERT_FLOAT_EQ(vec2.z, 2.f);

  vec2 = vec2 - vec1;
  ASSERT_FLOAT_EQ(vec2.x, 0.f);
  ASSERT_FLOAT_EQ(vec2.y, 0.f);
  ASSERT_FLOAT_EQ(vec2.z, 0.f);
  vec2 = vec2 * 2;
  ASSERT_FLOAT_EQ(vec2.x, 0.f);
  ASSERT_FLOAT_EQ(vec2.y, 0.f);
  ASSERT_FLOAT_EQ(vec2.z, 0.f);
}

TEST(Test, Vec3FunctionsTest) {
  ng::Vec3 vec1(2, 2, 2);
  ng::Vec3 vec2(2, 2, 2);
  {
    float result = vec1.Dot(vec2);
    ASSERT_FLOAT_EQ(result, 12);
  }
  {
    float result = vec1.Absolute();
    float correct = sqrtf(12);
    ASSERT_FLOAT_EQ(result, correct);
  }
  {
    float result1 = vec1.Angle(vec2);
    auto result2 = vec1.Cross(vec2);
    vec1.Normalize(1);
    (void)result1;
    (void)result2;
  }
}

TEST(Test, Normalize) {
  auto result = ng::FileReader::GetReader().Read(
      std::filesystem::absolute("tests/cube2.obj"));
  ng::Normalizer::GetNormalizer().Normalize(result.first, 1.f);
  const auto &vertexes = result.first.GetVertexes();
  std::vector<std::vector<float>> correct_vertexes{
      {0.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 0.f},
      {0.f, 1.f, 0.f}, {1.f, 1.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 1.f, 1.f},
      {0.f, 1.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 1.f, 1.f},
  };
  for (std::size_t i = 0; i < correct_vertexes.size(); i++) {
    ASSERT_FLOAT_EQ(vertexes[i].position.x, correct_vertexes[i][0]);
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
