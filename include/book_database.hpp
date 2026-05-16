#pragma once

#include <cstddef>
#include <initializer_list>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"
#include <unordered_set>

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using value_type = Book;
    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;
    using reverse_iterator = typename BookContainer::reverse_iterator;
    using reference = typename BookContainer::reference;
    using container_type = BookContainer;
    using difference_type = std::ptrdiff_t;
    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods
    iterator begin() { return books_.begin(); }
    iterator end() { return books_.end(); }
    reverse_iterator rbegin() { return books_.rbegin(); }
    reverse_iterator rend() { return books_.rend(); }
    const_iterator begin() const { return books_.begin(); };
    const_iterator end() const { return books_.end(); };
    size_t size() const { return books_.size(); };

    void PushBack(const Book &book) {
        books_.push_back(book);
        auto [it, _] = authors_.emplace(book.author);
        books_.back().author = std::string_view(*it);
    }
    void PushBack(Book &&book) {
        auto [it, _] = authors_.emplace(book.author);
        book.author = std::string_view(*it);
        books_.push_back(std::move(book));
    }

    template <typename... Args>
    requires std::constructible_from<Book, Args...>
    reference EmplaceBack(Args &&...args) {
        books_.emplace_back(std::forward<Args>(args)...);
        reference ref = books_.back();
        auto [it, _] = authors_.emplace(ref.author);
        ref.author = std::string_view(*it);
        return ref;
    }

    const BookContainer &GetBooks() const { return books_; }
    const AuthorContainer &GetAuthors() const { return authors_; }

    BookDatabase(std::initializer_list<Book> list) {
        if constexpr (requires { books_.reserve(list.size()); }) {
            books_.reserve(list.size());
        }

        for (auto &book : list) {
            PushBack(book);
        }
    }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb
