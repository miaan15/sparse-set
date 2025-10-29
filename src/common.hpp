#ifndef _COMMON_HPP
#define _COMMON_HPP

#include <ranges>

template <class R, class T>
concept container_compatible_range
    = std::ranges::input_range<R>
      && (std::convertible_to<T, std::ranges::range_reference_t<R>>
          || std::constructible_from<T, std::ranges::range_reference_t<R>>
          || std::convertible_to<T, std::ranges::range_rvalue_reference_t<R>>
          || std::constructible_from<T, std::ranges::range_rvalue_reference_t<R>>);

#endif
