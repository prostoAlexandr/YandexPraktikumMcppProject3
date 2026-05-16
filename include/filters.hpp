#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

class YearBetween {
public:
    YearBetween(int b, int e) : begin_(b), end_(e) {}
    bool operator()(const Book &book) const { return book.year >= begin_ && book.year <= end_; }

private:
    int begin_;
    int end_;
};

class RatingAbove {
public:
    RatingAbove(double r) : rating_(r) {}
    bool operator()(const Book &book) const { return book.rating > rating_; }

private:
    double rating_;
};

class GenreIs {
public:
    GenreIs(Genre g) : genre_(g) {}
    bool operator()(const Book &book) const { return book.genre == genre_; }

private:
    Genre genre_;
};

template <BookPredicate... Args>
auto all_of(Args &&...args) {
    return [... args = std::forward<Args>(args)](const auto &book) { return (args(book) && ...); };
}

template <BookPredicate... Args>
auto any_of(Args &&...args) {
    return [... args = std::forward<Args>(args)](const auto &book) { return (args(book) || ...); };
}

template <BookIterator T, typename Pred>
auto filterBooks(const T cbegin, const T cend, Pred p) {
    std::vector<std::reference_wrapper<const Book>> result;
    std::for_each(cbegin, cend, [&result, &p](const auto &book) {
        if (p(book)) {
            result.push_back(std::ref(book));
        }
    });
    return result;
}

}  // namespace bookdb