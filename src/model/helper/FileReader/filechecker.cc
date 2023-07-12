#include "filechecker.h"

namespace ng {
ObjectFileType FileChecker::DetectFileType() {
  std::ifstream file(path_);
  if (!file.is_open()) {
    return kWithError;
  }

  std::string line;
  std::string prefix;
  std::stringstream ss;

  bool check_is_finished = false;
  bool position_coords_exists = false, texture_coords_exists = false,
       normal_coords_exists = false, facets_exists = false;
  while (std::getline(file, line) && !check_is_finished) {
    ss.clear();
    ss.str(line);
    ss >> prefix;

    if (!position_coords_exists && prefix == "v") {
      position_coords_exists = true;
    } else if (prefix == "vt") {
      texture_coords_exists = true;
    } else if (prefix == "vn") {
      normal_coords_exists = true;
    } else if (!facets_exists && prefix == "f") {
      facets_exists = true;
    }

    if (facets_exists && normal_coords_exists && texture_coords_exists &&
        position_coords_exists) {
      check_is_finished = true;
    }
  }

  file.close();
  return DetectFileTypeFromParameters(position_coords_exists,
                                      texture_coords_exists,
                                      normal_coords_exists, facets_exists);
}

ObjectFileType FileChecker::DetectFileTypeFromParameters(
    bool position_coords_exists, bool texture_coords_exists,
    bool normal_coords_exists, bool facets_exists) {
  ObjectFileType object_file_type{};
  if (!position_coords_exists || !facets_exists) {
    object_file_type = kWithError;
  } else if (position_coords_exists && facets_exists) {
    if (texture_coords_exists && normal_coords_exists) {
      object_file_type = kWithAll;
    } else if (texture_coords_exists && !normal_coords_exists) {
      object_file_type = kWithTexture;
    } else if (!texture_coords_exists && normal_coords_exists) {
      object_file_type = kWithNormal;
    } else {
      object_file_type = kWithoutTextureNormal;
    }
  }
  return object_file_type;
}

}  // namespace ng
