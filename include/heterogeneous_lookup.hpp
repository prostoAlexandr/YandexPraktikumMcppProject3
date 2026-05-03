#pragma once

#include <string>
#include <string_view>
#include "book.hpp"
#include "comparators.hpp"

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;
    bool operator()(std::string_view sv, const Book& book) const
    {
        return sv.compare(book.author) < 0;
    }
    bool operator()(const Book& book, std::string_view sv) const
    {
        return book.author.compare(sv) < 0;
    }
    bool operator()(std::string_view lhs, std::string_view rhs) const
    {
        return lhs.compare(rhs) < 0;
    }
};

struct TransparentGenreLess {
    using is_transparent = void;
    bool operator()(Genre g, const Book& book) const
    {
        return g < book.genre;
    }
    bool operator()(const Book& book, Genre g) const
    {
        return book.genre < g;
    }
    bool operator()(Genre lhs, Genre rhs) const
    {
        return lhs < rhs;
    }
};

struct TransparentStringEqual {
    using is_transparent = void;
};

struct TransparentStringHash {
    using is_transparent = void;
};

}  // namespace bookdb
