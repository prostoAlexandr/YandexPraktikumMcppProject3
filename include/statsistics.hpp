#pragma once

#include <algorithm>
#include <cstddef>
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

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string_view, int, Comparator> map;
    std::for_each(cont.begin(), cont.end(), [&map](const auto &book) {
        auto [it, _] = map.try_emplace(book.author, 0);
        ++it->second;
    });
    return map;
}

namespace {

struct RatingCounter {
    double rating = 0;
    size_t counter = 0;
};

}  // namespace

template <BookIterator It, typename Comparator = TransparentGenreLess>
auto calculateGenreRatings(const It cbegin, const It cend, Comparator comp = {}) {
    std::flat_map<Genre, RatingCounter, Comparator> map;
    std::for_each(cbegin, cend, [&map](const auto &book) {
        auto [it, _] = map.try_emplace(book.genre, RatingCounter{0, 0});
        it->second.rating += book.rating;
        ++it->second.counter;
    });

    std::flat_map<std::string_view, double> result;
    std::for_each(map.begin(), map.end(), [&result, &map](const auto &rc) {
        result.emplace(GenreToString(rc.first), rc.second.rating / rc.second.counter);
    });
    return result;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &cont) {
    auto sum_rating =
        // Также можно использовать мультилямбду
        std::reduce(cont.begin(), cont.end(), 0.0, [](double val, const auto &book) { return val + double(book); });
    return cont.size() ? sum_rating / cont.size() : 0;
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t count) {
    std::vector<std::reference_wrapper<const Book>> result;
    std::sample(cont.begin(), cont.end(), std::back_inserter(result), count, std::mt19937{std::random_device{}()});
    return result;
}

template <BookContainerLike T, BookComparator Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t count, Comparator comp = {}) {
    count = std::min(count, cont.size());

    auto nthElementIt = std::prev(cont.end(), count);
    std::nth_element(cont.begin(), nthElementIt, cont.end(), comp);
    std::sort(nthElementIt, cont.end(), comp);

    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(count);
    std::transform(cont.rbegin(), cont.rbegin() + count, std::back_inserter(result),
                   [](const auto &val) { return std::ref(val); });
    return result;
}

}  // namespace bookdb
