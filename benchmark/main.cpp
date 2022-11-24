#include <benchmark/benchmark.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <ranges>
#include <sorters/counting_sort.hpp>
#include <sorters/quick_sort.hpp>
#include <utils/array.hpp>
#include <utils/assets.hpp>

static void custom_arguments(benchmark::internal::Benchmark* b) {
  // Get the sizes of the array
  auto elements = utils::assets::get_size_of_assets("assets/asc");
  // elements.resize(1);

  for (auto size : elements) b->Args({size});
}

template <typename... Args>
static void BM_CountingSort(benchmark::State& state, Args&&... args) {
  auto args_tuple = std::make_tuple(std::move(args)...);
  for (auto _ : state) {
    state.PauseTiming();
    auto array = utils::Array::vector_from_file<uint32_t>(fmt::format(
        "assets/{}{}.txt", std::get<0>(args_tuple), state.range(0)));
    state.ResumeTiming();
    sorters::CountingSort::sort(array);
  }
}

template <typename... Args>
static void BM_QuickSort(benchmark::State& state, Args&&... args) {
  auto args_tuple = std::make_tuple(std::move(args)...);
  for (auto _ : state) {
    state.PauseTiming();
    auto array = utils::Array::vector_from_file<uint32_t>(fmt::format(
        "assets/{}{}.txt", std::get<0>(args_tuple), state.range(0)));
    state.ResumeTiming();
    sorters::QuickSort::sort(array);
  }
}

BENCHMARK_CAPTURE(BM_CountingSort, asc, "asc/o")
    ->Apply(custom_arguments)
    ->Complexity();
BENCHMARK_CAPTURE(BM_CountingSort, dec, "dec/d")
    ->Apply(custom_arguments)
    ->Complexity();
BENCHMARK_CAPTURE(BM_CountingSort, part_sorted, "part_sorted/po")
    ->Apply(custom_arguments)
    ->Complexity();
BENCHMARK_CAPTURE(BM_CountingSort, rand, "random/a")
    ->Apply(custom_arguments)
    ->Complexity();

BENCHMARK_CAPTURE(BM_QuickSort, asc, "asc/o")
    ->Apply(custom_arguments)
    ->Complexity();
BENCHMARK_CAPTURE(BM_QuickSort, dec, "dec/d")
    ->Apply(custom_arguments)
    ->Complexity();
BENCHMARK_CAPTURE(BM_QuickSort, part_sorted, "part_sorted/po")
    ->Apply(custom_arguments)
    ->Complexity();
BENCHMARK_CAPTURE(BM_QuickSort, rand, "random/a")
    ->Apply(custom_arguments)
    ->Complexity();

BENCHMARK_MAIN();
