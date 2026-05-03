#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T t) {
    t.begin();
    t.end();
    t.rbegin();
    t.rend();
    t.size();
    t.clear();
    requires std::is_same_v<typename T::value_type, Book>;
    requires requires { t.emplace_back(std::declval<typename T::value_type>()); };
    requires requires { t.push_back(std::declval<typename T::value_type>()); };
};

template <typename T>
concept BookIterator = std::input_iterator<T>;

template <typename S, typename I>
concept BookSentinel = true;

template <typename P>
concept BookPredicate = requires(const P p) { requires std::is_convertible_v<typeof(p(std::declval<Book>())), bool>; };

template <typename C>
concept BookComparator = requires(const C c) { requires std::is_convertible_v<typeof(c(std::declval<Book>(), std::declval<Book>())), bool>; };

}  // namespace bookdb