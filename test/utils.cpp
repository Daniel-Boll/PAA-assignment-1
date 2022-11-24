#include <fmt/core.h>
#include <fmt/ranges.h>
#include <gtest/gtest.h>

#include <utils/array.hpp>
#include <utils/assets.hpp>

TEST(ArrayUtils, FromFile) {
  auto array =
      utils::Array::vector_from_file<uint32_t>("./assets/asc/o100.txt");
  ASSERT_EQ(array.size(), 100);
}

TEST(AssetsUtils, GetSizeOfAssets) {
  auto sizes = utils::assets::get_size_of_assets("./assets/asc");

  // Sort output of ls
  std::string command =
      "ls -1 ./assets/asc | awk -F 'o' '{print $2}' | awk -F '.' '{print $1}'";

  FILE* pipe = popen(command.c_str(), "r");
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }

  std::vector<int32_t> expected_sizes;
  char buffer[128];
  while (!feof(pipe)) {
    if (fgets(buffer, 128, pipe) != nullptr) {
      expected_sizes.push_back(std::stoi(buffer));
    }
  }
  pclose(pipe);
  std::ranges::sort(expected_sizes);

  ASSERT_EQ(sizes, expected_sizes);
}
