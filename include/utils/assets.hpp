#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <regex>
#include <string>
#include <utils/array.hpp>
#include <vector>

namespace utils::assets {
  inline std::vector<int32_t> get_size_of_assets(const std::string& path) {
    std::vector<int32_t> sizes;

    // Read the name of the files from path
    std::filesystem::path p(path);
    for (const auto& entry : std::filesystem::directory_iterator(p)) {
      // Extract only the number from the name of the file
      std::string name = entry.path().filename().string();
      std::regex re("[0-9]+");
      std::smatch match;
      std::regex_search(name, match, re);

      // Convert the number to int32_t and add it to the vector
      int32_t size = std::stoi(match[0]);
      sizes.push_back(size);
    }

    std::ranges::sort(sizes);
    return sizes;
  }

  inline std::vector<std::vector<int32_t>> get_vectors_from_assets(
      const std::string& path) {
    std::vector<std::vector<int32_t>> vectors;

    // Read the name of the files from path

    std::filesystem::path p(path);
    for (const auto& entry : std::filesystem::directory_iterator(p)) {
      // Extract only the number from the name of the file
      std::string name = entry.path();

      vectors.push_back(utils::Array::vector_from_file<int32_t>(name));
    }

    return vectors;
  }
}  // namespace utils::assets
