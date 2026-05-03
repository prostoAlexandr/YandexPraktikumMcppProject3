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

struct TransparentStringEqual {
    using is_transparent = void;
};

struct TransparentStringHash {
    using is_transparent = void;
};

}  // namespace bookdb
