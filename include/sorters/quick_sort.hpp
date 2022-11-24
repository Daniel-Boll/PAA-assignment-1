#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <ranges>
#include <utility>
#include <vector>

// Get the iterator as the std::ranges version

static std::random_device rd;
static std::mt19937 rng{rd()};

static auto get_middle_pivot = [](auto begin, auto end) {
  auto distance = std::distance(begin, end);
  auto middle = std::next(begin, distance / 2);

  return middle;
};

static auto get_random_pivot = [](auto begin, auto end) {
  std::uniform_int_distribution<std::size_t> dist(
      0, std::ranges::distance(begin, end) - 1);
  return begin + dist(rng);
};

static auto get_mean_pivot = [](auto begin, auto end) {
  std::uniform_int_distribution<std::size_t> dist(
      0, std::ranges::distance(begin, end) - 1);
  int32_t first = std::distance(begin, begin + dist(rng));
  int32_t second = std::distance(begin, begin + dist(rng));
  int32_t third = std::distance(begin, begin + dist(rng));

  // Return the iterator of the mean
  return begin + (first + second + third) / 3;
};

namespace sorters {
  class QuickSort {
   private:
    std::uniform_int_distribution<uint32_t> dist;

   public:
    template <typename T, typename compare = std::less<>>
    requires requires(T a) {
      { compare()(a, a) } -> std::convertible_to<bool>;
      { std::ranges::begin(a) } -> std::convertible_to<typename T::iterator>;
      { std::ranges::end(a) } -> std::convertible_to<typename T::iterator>;
    }
    static void sort(T& array, compare comp = compare()) {
      quick_sort(array.begin(), array.end(), comp);
    }

   private:
    template <typename iterator, typename compare>
    static void quick_sort(iterator begin, iterator end, compare comp) {
      if (std::distance(begin, end) <= 1) return;

      auto boundary = partition(begin, end, comp);
      quick_sort(begin, boundary, comp);
      quick_sort(std::next(boundary), end, comp);
    }

    template <typename iterator, typename compare>
    static iterator partition(iterator begin, iterator end, compare comp) {
      auto pivot = get_mean_pivot(begin, end);

      std::iter_swap(pivot, std::prev(end));

      auto boundary = begin;
      for (auto i = begin; i != std::prev(end); ++i) {
        if (comp(*i, *std::prev(end))) {
          std::iter_swap(i, boundary);
          ++boundary;
        }
      }

      std::iter_swap(boundary, std::prev(end));

      return boundary;
    }
  };
}  // namespace sorters
