#ifndef _EMG_UTILITIES_SPARSE_SET_HPP
#define _EMG_UTILITIES_SPARSE_SET_HPP

#include <cmath>
#include <memory>
#include <ranges>
#include <vector>

template <typename T, typename Hash = std::hash<T>,
          typename KeyEqual = std::equal_to<T>,
          typename Allocator = std::allocator<T>>
class sparse_set {
public:
  using key_type = T;
  using value_type = T;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using hasher = Hash;
  using key_equal = KeyEqual;
  using allocator_type = Allocator;

public:
  sparse_set() : dense_arr(), sparse_arr(INIT_CAPACITY) {}
  ~sparse_set() = default;

  sparse_set(const sparse_set &) = default;
  auto operator=(const sparse_set &) -> sparse_set & = default;

  sparse_set(sparse_set &&) noexcept = default;
  auto operator=(sparse_set &&) noexcept -> sparse_set & = default;

public:
  [[nodiscard]] auto size() const -> size_type { return dense_arr.size(); }

  auto insert(const value_type &value) -> bool {
    if (contains(value))
      return false;

    if (size() > std::floor(capacity * CAPACITY_LOAD_FACTOR)) {
      rehash(capacity * CAPACITY_GROW);
    }

    sparse_arr[hash_value(value)] = size();
    dense_arr.insert(value);
  }
  auto erase(const value_type &value) -> size_type {
    if (!contains(value))
      return 0;

    size_type hashed = hash_value(value);
    sparse_arr[hash_value(dense_arr[size() - 1])] = sparse_arr[hashed];
    dense_arr[sparse_arr[hashed]] = std::move(dense_arr[size() - 1]);
  }
  auto contains(const value_type &value) const -> bool {
    size_type hashed = hash_value(value);
    size_type p = sparse_arr[hashed];
    return p >= 0 && p < size() && hash_value(dense_arr[p]) == hashed;
  }

private:
  std::vector<value_type, allocator_type> dense_arr;
  std::vector<size_type, typename std::allocator_traits<
                             allocator_type>::template rebind_alloc<size_type>>
      sparse_arr;

  size_type capacity{INIT_CAPACITY};

  static constexpr size_type INIT_CAPACITY = 64;
  static constexpr double CAPACITY_LOAD_FACTOR = 0.75;
  static constexpr double CAPACITY_GROW = 1.5;

private:
  // FIXME: bias hashing
  auto hash_value(value_type &value) -> size_type {
    return hasher(value) % capacity;
  }

  auto rehash(size_type new_capacity) -> void {
    sparse_arr.resize(new_capacity);
    capacity = new_capacity;
    for (auto [idx, value] : std::views::enumerate(dense_arr)) {
      sparse_arr[hash_value(value)] = idx;
    }
  }
};

#endif
