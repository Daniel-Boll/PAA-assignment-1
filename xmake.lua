set_languages("c++20")
add_rules("mode.debug", "mode.release")

local libs = { "benchmark", "gtest", "fmt" }
add_requires(table.unpack(libs))
add_includedirs("include")

target("bench")
  set_kind("binary")
  add_files("benchmark/main.cpp")
  add_packages(table.unpack(libs))

target("test")
  set_kind("binary")
  add_files("test/*.cpp")
  add_packages(table.unpack(libs))
