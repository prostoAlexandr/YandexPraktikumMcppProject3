#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T t, typename T::value_type b) {
    t.begin();
    t.end();
    t.rbegin();
    t.rend();
    t.size();
    t.back();
    t.clear();
    t.emplace_back(b);
    t.push_back(b);
    requires std::is_same_v<typename T::value_type, Book>;
};

template <typename T>
concept BookIterator = std::input_iterator<T>;

template <typename S, typename I>
concept BookSentinel = true;

template <typename P>
concept BookPredicate = requires(const P p, const Book &b) {
    { p(b) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(const C c, const Book &b1, const Book &b2) {
    { c(b1, b2) } -> std::convertible_to<bool>;
};

}  // namespace bookdb