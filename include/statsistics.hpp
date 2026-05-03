#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>

#include "book.hpp"
#include "book_database.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

#include <flat_map>
#include <print>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string_view, int, Comparator> map;
    std::for_each(cont.begin(), cont.end(), [&map](const auto &book) {
        auto it = map.find(book);
        if (it != map.end()) {
            it->second++;
        } else {
            map.emplace(book.author, 1);
        }
    });
    return map;
}

namespace {

struct RatingCounter {
    double rating = 0;
    size_t counter = 0;
};

}  // namespace

template <BookIterator It, typename Comparator = TransparentStringLess>
auto calculateGenreRatings(const It cbegin, const It cend, Comparator comp = {}) {
    std::flat_map<std::string_view, RatingCounter, Comparator> map;
    std::for_each(cbegin, cend, [&map](const auto &book) {
        auto it = map.find(book);
        if (it == map.end()) {
            map.emplace(book.author, RatingCounter{book.rating, 1});
            return;
        }

        it->second.rating += book.rating;
        ++it->second.counter;
    });

    std::flat_map<std::string_view, double> result;
    std::for_each(map.begin(), map.end(),
                  [&result, &map](const auto &rc) { result.emplace(rc.first, rc.second.rating / rc.second.counter); });
    return result;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &cont) {
    auto sum_rating =
        std::accumulate(cont.begin(), cont.end(), 0.0, [](double val, const auto &book) { return val + book.rating; });
    return sum_rating / cont.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t count) {
    std::vector<std::reference_wrapper<const Book>> result;
    std::sample(cont.begin(), cont.end(), std::back_inserter(result), count, std::mt19937{std::random_device{}()});
    return result;
}

template <BookContainerLike T, BookComparator Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t count, Comparator comp = {}) {
    std::sort(cont.begin(), cont.end(), comp);
    std::vector<std::reference_wrapper<const Book>> result;
    std::transform(cont.rbegin(), cont.rbegin() + count, std::back_inserter(result),
                   [](const auto &val) { return std::ref(val); });
    return result;
}

}  // namespace bookdb
