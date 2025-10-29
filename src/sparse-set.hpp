#ifndef _SPARSE_SET_HPP
#define _SPARSE_SET_HPP

#include <cmath>
#include <initializer_list>
#include <memory>
#include <ranges>
#include <vector>

#ifndef INIT_SPARSE_SIZE
#    define INIT_SPARSE_SIZE 32
#endif
#ifndef LOAD_FACTOR
#    define LOAD_FACTOR 0.7
#endif
#ifndef SPARSE_SIZE_GROW
#    define SPARSE_SIZE_GROW 2
#endif

template <class R, class T>
concept container_compatible_range
    = std::ranges::input_range<R>
      && (std::convertible_to<T, std::ranges::range_reference_t<R>>
          || std::constructible_from<T, std::ranges::range_reference_t<R>>
          || std::convertible_to<T, std::ranges::range_rvalue_reference_t<R>>
          || std::constructible_from<T, std::ranges::range_rvalue_reference_t<R>>);

template <
    typename T,
    typename Hash      = std::hash<T>,
    typename KeyEqual  = std::equal_to<T>,
    typename Allocator = std::allocator<T>>
class sparse_set {
public:
    using key_type       = T;
    using value_type     = T;
    using hasher         = Hash;
    using key_equal      = KeyEqual;
    using allocator_type = Allocator;

private:
    using dense_arr_type = std::vector<value_type, allocator_type>;

    struct sparse_arr_entry {
        size_t pos;
        size_t dist{0};
    };
    using sparse_arr_type = std::vector<
        sparse_arr_entry,
        typename std::allocator_traits<allocator_type>::template rebind_alloc<sparse_arr_entry>>;

public:
    using iterator               = typename dense_arr_type::iterator;
    using const_iterator         = typename dense_arr_type::const_iterator;
    using reverse_iterator       = typename dense_arr_type::reverse_iterator;
    using const_reverse_iterator = typename dense_arr_type::const_reverse_iterator;

public:
    sparse_set()
      : dense_arr(), sparse_arr(INIT_SPARSE_SIZE) {}
    ~sparse_set() = default;

    sparse_set(const sparse_set &)                     = default;
    auto operator=(const sparse_set &) -> sparse_set & = default;

    sparse_set(sparse_set &&) noexcept                     = default;
    auto operator=(sparse_set &&) noexcept -> sparse_set & = default;

public:
    [[nodiscard]] auto size() const -> size_t { return dense_arr.size(); }
    [[nodiscard]] auto empty() const -> bool { return dense_arr.empty(); }
    [[nodiscard]] auto capacity() const -> size_t { return dense_arr.capacity(); }

    [[nodiscard]] auto sparse_size() const -> size_t { return sparse_arr.size(); }

    auto begin() -> iterator { return dense_arr.begin(); }
    auto end() -> iterator { return dense_arr.end(); }
    auto begin() const -> const_iterator { return dense_arr.begin(); }
    auto end() const -> const_iterator { return dense_arr.end(); }
    auto cbegin() const -> const_iterator { return dense_arr.cbegin(); }
    auto cend() const -> const_iterator { return dense_arr.cend(); }

    auto rbegin() -> reverse_iterator { return dense_arr.rbegin(); }
    auto rend() -> reverse_iterator { return dense_arr.rend(); }
    auto rbegin() const -> const_reverse_iterator { return dense_arr.rbegin(); }
    auto rend() const -> const_reverse_iterator { return dense_arr.rend(); }
    auto crbegin() const -> const_reverse_iterator { return dense_arr.crbegin(); }
    auto crend() const -> const_reverse_iterator { return dense_arr.crend(); }

    auto clear() noexcept -> void;

    auto insert(const value_type &value) -> std::pair<iterator, bool>;
    auto insert(value_type &&value) -> std::pair<iterator, bool>;
    template <class InputIt>
    auto insert(InputIt first, InputIt last) -> void;
    auto insert(std::initializer_list<value_type> ilist) -> void;

    auto insert_range(container_compatible_range<value_type> auto &&rg) -> void;

    template <class... Args>
    auto emplace(Args &&...args) -> std::pair<iterator, bool>;

    auto erase(const value_type &value) -> size_t;

    auto find(const value_type &value) -> iterator;
    auto find(const value_type &value) const -> const_iterator;
    auto count(const value_type &value) const -> size_t;
    auto contains(const value_type &value) const -> bool;

    auto swap(sparse_set &other) noexcept(
        std::allocator_traits<allocator_type>::is_always_equal::value
        && std::is_nothrow_swappable_v<hasher> && std::is_nothrow_swappable_v<key_equal>
    ) -> void;

    auto rehash(size_t new_sparse_size) -> void;

    auto reserve(size_t count) -> void;

private:
    dense_arr_type  dense_arr;
    sparse_arr_type sparse_arr;

private:
    auto hash(const value_type &value) const -> size_t { return hasher{}(value) % sparse_size(); }

    auto insert_sparse_by_pos(size_t pos) -> void;
    auto find_sparse_by_value(const value_type &value) const -> size_t;
    auto remove_sparse_by_hash(size_t hashed) -> void;
};

#define _sparse_set_def sparse_set<T, Hash, KeyEqual, Allocator>

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::clear() noexcept -> void {
    dense_arr.clear();
    std::fill(sparse_arr.begin(), sparse_arr.end(), sparse_arr_entry{.pos = 0, .dist = 0});
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::insert(const value_type &value) -> std::pair<iterator, bool> {
    if (!dense_arr.empty() && contains(value)) return {end(), false};

    if (size()
        >= static_cast<size_t>(std::floor(static_cast<double>(sparse_size()) * LOAD_FACTOR))) {
        rehash(sparse_size() * SPARSE_SIZE_GROW);
    }

    dense_arr.push_back(value);
    insert_sparse_by_pos(size() - 1);

    return {end() - 1, true};
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::insert(value_type &&value) -> std::pair<iterator, bool> {
    if (!dense_arr.empty() && contains(value)) return {end(), false};

    if (size()
        >= static_cast<size_t>(std::floor(static_cast<double>(sparse_size()) * LOAD_FACTOR))) {
        rehash(sparse_size() * SPARSE_SIZE_GROW);
    }

    dense_arr.push_back(std::move(value));
    insert_sparse_by_pos(size() - 1);

    return {end() - 1, true};
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
template <class InputIt>
inline auto _sparse_set_def::insert(InputIt first, InputIt last) -> void {
    for (; first != last; ++first) {
        auto &&v = *first;
        (void)insert(std::forward<decltype(v)>(v));
    }
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::insert(std::initializer_list<value_type> ilist) -> void {
    insert(ilist.begin(), ilist.end());
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::insert_range(container_compatible_range<value_type> auto &&rg)
    -> void {
    for (auto &&v : rg) {
        (void)insert(std::forward<decltype(v)>(v));
    }
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
template <class... Args>
inline auto _sparse_set_def::emplace(Args &&...args) -> std::pair<iterator, bool> {
    value_type value{std::forward<Args>(args)...};
    if (!dense_arr.empty() && contains(value)) return {end(), false};

    if (size()
        >= static_cast<size_t>(std::floor(static_cast<double>(sparse_size()) * LOAD_FACTOR))) {
        rehash(sparse_size() * SPARSE_SIZE_GROW);
    }

    dense_arr.emplace_back(std::move(value));
    insert_sparse_by_pos(size() - 1);

    return {end() - 1, true};
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::erase(const value_type &value) -> size_t {
    if (dense_arr.empty() || !contains(value)) return 0;

    size_t hashed = find_sparse_by_value(value);
    if (hashed == sparse_size()) return 0;

    size_t pos = sparse_arr[hashed].pos;

    if (pos != size() - 1) {
        size_t back_hashed = find_sparse_by_value(dense_arr.back());
        if (back_hashed < sparse_size()) {
            sparse_arr[back_hashed].pos = pos;
        }
    }
    remove_sparse_by_hash(hashed);

    dense_arr[pos] = std::move(dense_arr.back());
    dense_arr.pop_back();

    return 1;
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::find(const value_type &value) -> iterator {
    size_t hashed = find_sparse_by_value(value);
    if (hashed == sparse_size()) return end();
    size_t pos = sparse_arr[hashed].pos;
    return dense_arr.begin() + static_cast<std::ptrdiff_t>(pos);
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::find(const value_type &value) const -> const_iterator {
    size_t hashed = find_sparse_by_value(value);
    if (hashed == sparse_size()) return end();
    size_t pos = sparse_arr[hashed].pos;
    return dense_arr.begin() + static_cast<std::ptrdiff_t>(pos);
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::count(const value_type &value) const -> size_t {
    size_t hashed = find_sparse_by_value(value);
    return (hashed < sparse_size() ? 1 : 0);
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::contains(const value_type &value) const -> bool {
    size_t hashed = find_sparse_by_value(value);
    return hashed < sparse_size();
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::swap(sparse_set &other) noexcept(
    std::allocator_traits<allocator_type>::is_always_equal::value
    && std::is_nothrow_swappable_v<hasher> && std::is_nothrow_swappable_v<key_equal>
) -> void {
    dense_arr.swap(other.dense_arr);
    sparse_arr.swap(other.sparse_arr);
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::rehash(size_t new_sparse_size) -> void {
    std::fill(sparse_arr.begin(), sparse_arr.end(), sparse_arr_entry{.pos = 0, .dist = 0});
    sparse_arr.resize(new_sparse_size);
    for (auto idx : std::views::iota(0U, dense_arr.size())) {
        insert_sparse_by_pos(idx);
    }
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::reserve(size_t count) -> void {
    dense_arr.reserve(count);
    if (count >= static_cast<size_t>(static_cast<double>(sparse_arr.capacity()) * LOAD_FACTOR)) {
        sparse_arr.reserve(static_cast<size_t>(static_cast<double>(count) / LOAD_FACTOR));
    }
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::insert_sparse_by_pos(size_t pos) -> void {
    size_t           hashed = hash(dense_arr[pos]);
    sparse_arr_entry entry{.pos = pos, .dist = 1};

    while (true) {
        auto &slot = sparse_arr[hashed];
        if (slot.dist == 0) {
            slot = entry;
            return;
        }

        if (slot.dist < entry.dist) {
            std::swap(slot, entry);
        }

        entry.dist++;
        hashed = (hashed + 1) % sparse_size();
    }
    std::unreachable();
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::find_sparse_by_value(const value_type &value) const -> size_t {
    size_t hashed = hash(value);
    size_t dist   = 1;
    while (true) {
        const auto &slot = sparse_arr[hashed];
        if (slot.dist == 0 || dist > slot.dist) return sparse_size();
        if (key_equal{}(dense_arr[slot.pos], value)) return hashed;
        dist++;
        hashed = (hashed + 1) % sparse_size();
    }
    std::unreachable();
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
inline auto _sparse_set_def::remove_sparse_by_hash(size_t hashed) -> void {
    size_t curr = hashed;

    while (true) {
        size_t next      = (curr + 1) % sparse_size();
        auto  &next_slot = sparse_arr[next];

        if (next_slot.dist <= 1) {
            sparse_arr[curr].dist = 0;
            return;
        }

        sparse_arr[curr] = next_slot;
        sparse_arr[curr].dist--;

        curr = next;
    }
}

template <typename T, typename Hash, typename KeyEqual, typename Allocator>
auto swap(
    sparse_set<T, Hash, KeyEqual, Allocator> &lhs, sparse_set<T, Hash, KeyEqual, Allocator> &rhs
) noexcept(noexcept(lhs.swap(rhs))) -> void {
    lhs.swap(rhs);
}

#undef _sparse_set_def

#endif
