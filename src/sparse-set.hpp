#ifndef _EMG_UTILITIES_SPARSE_SET_HPP
#define _EMG_UTILITIES_SPARSE_SET_HPP

#include <cmath>
#include <memory>
#include <ranges>
#include <vector>

#ifndef INIT_SPARSE_SIZE
#    define INIT_SPARSE_SIZE 64
#endif
#ifndef LOAD_FACTOR
#    define LOAD_FACTOR 0.7
#endif
#ifndef SPARSE_SIZE_GROW
#    define SPARSE_SIZE_GROW 1.5
#endif

template <
    typename T,
    typename Hash      = std::hash<T>,
    typename KeyEqual  = std::equal_to<T>,
    typename Allocator = std::allocator<T>>
class sparse_set {
public:
    using key_type        = T;
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = std::ptrdiff_t;
    using hasher          = Hash;
    using key_equal       = KeyEqual;
    using allocator_type  = Allocator;

private:
    using dense_arr_type = std::vector<value_type, allocator_type>;

    struct sparse_arr_entry {
        size_type pos;
        size_type dist{0};
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
    [[nodiscard]] auto size() const -> size_type { return dense_arr.size(); }
    [[nodiscard]] auto empty() const -> bool { return dense_arr.empty(); }

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

    auto clear() noexcept -> void {
        dense_arr.clear();
        std::fill(sparse_arr.begin(), sparse_arr.end(), sparse_arr_entry{.pos = 0, .dist = 0});
    }

    auto insert(const value_type &value) -> std::pair<iterator, bool> {
        if (!dense_arr.empty() && contains(value)) return {end(), false};

        if (size() > std::floor(sparse_size * LOAD_FACTOR)) {
            rehash(sparse_size * SPARSE_SIZE_GROW);
        }

        dense_arr.push_back(value);
        insert_sparse_arr(size() - 1);

        return {end() - 1, true};
    }
    auto insert(value_type &&value) noexcept -> std::pair<iterator, bool> {
        if (!dense_arr.empty() && contains(value)) return {end(), false};

        if (size() > std::floor(sparse_size * LOAD_FACTOR)) {
            rehash(sparse_size * SPARSE_SIZE_GROW);
        }

        dense_arr.push_back(std::move(value));
        insert_sparse_arr(size() - 1);

        return {end() - 1, true};
    }
    template <class... Args>
    auto emplace(Args &&...args) -> std::pair<iterator, bool> {
        return insert(value_type{std::forward<Args>(args)...});
    }

    auto erase(const value_type &value) -> size_type {
        if (dense_arr.empty() || !contains(value)) return 0;

        size_type hashed = find_sparse_arr(value).value_or(sparse_size);
        if (hashed == sparse_size) return 0;

        size_type pos = sparse_arr[hashed].pos;

        if (pos != size() - 1) {
            size_type back_hashed = find_sparse_arr(dense_arr.back()).value_or(sparse_size);
            if (back_hashed < sparse_size) {
                sparse_arr[back_hashed].pos = pos;
            }
        }

        dense_arr[pos] = std::move(dense_arr.back());
        dense_arr.pop_back();

        remove_sparse_arr(hashed);

        return 1;
    }

    auto find(const value_type &value) -> iterator {
        size_type hashed = find_sparse_arr(value).value_or(sparse_size);
        if (hashed == sparse_size) return end();
        size_type pos = sparse_arr[hashed].pos;
        return dense_arr.begin() + pos;
    }
    auto find(const value_type &value) const -> const_iterator {
        size_type hashed = find_sparse_arr(value).value_or(sparse_size);
        if (hashed == sparse_size) return end();
        size_type pos = sparse_arr[hashed].pos;
        return dense_arr.begin() + pos;
    }
    auto count(const value_type &value) const -> size_type {
        size_type hashed = find_sparse_arr(value).value_or(sparse_size);
        return (hashed < sparse_size ? 1 : 0);
    }
    auto contains(const value_type &value) const -> bool {
        size_type hashed = find_sparse_arr(value).value_or(sparse_size);
        return hashed < sparse_size;
    }

    auto rehash(size_type new_sparse_size) -> void {
        std::fill(sparse_arr.begin(), sparse_arr.end(), sparse_arr_entry{.pos = 0, .dist = 0});
        sparse_arr.resize(new_sparse_size);
        sparse_size = new_sparse_size;
        for (auto [idx, value] : std::views::enumerate(dense_arr)) {
            insert_sparse_arr(idx);
        }
    }

    auto reserve(size_type count) -> void { dense_arr.reserve(count); }

private:
    dense_arr_type  dense_arr;
    sparse_arr_type sparse_arr;

    size_type sparse_size{INIT_SPARSE_SIZE};

private:
    auto hash(const value_type &value) const -> size_type { return hasher{}(value) % sparse_size; }

    auto insert_sparse_arr(size_type pos) -> void {
        size_type        hashed = hash(dense_arr[pos]);
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
            hashed = (hashed + 1) % sparse_size;
        }
        std::unreachable();
    }
    [[nodiscard]] auto find_sparse_arr(const value_type &value) const -> std::optional<size_type> {
        size_type hashed = hash(value);
        size_type dist   = 1;
        while (true) {
            const auto &slot = sparse_arr[hashed];
            if (slot.dist == 0 || dist > slot.dist) return {};
            if (key_equal{}(dense_arr[slot.pos], value)) return hashed;
            dist++;
            hashed = (hashed + 1) % sparse_size;
        }
        std::unreachable();
    }
    auto remove_sparse_arr(size_type hashed) -> void {
        size_type curr = hashed;

        while (true) {
            size_type next      = (curr + 1) % sparse_size;
            auto     &next_slot = sparse_arr[next];

            if (next_slot.dist <= 1) {
                sparse_arr[curr].dist = 0;
                return;
            }

            sparse_arr[curr] = next_slot;
            sparse_arr[curr].dist--;

            curr = next;
        }
    }
};

#endif
