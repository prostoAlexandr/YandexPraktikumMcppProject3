#include <vector>
#include <flat_map>
#include "book.hpp"
#include "book_database.hpp"

namespace std {

template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        return format_to(fc.out(), "{}", bookdb::GenreToString(g));
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book &b, FormatContext &fc) const {
        return format_to(fc.out(), "Author: {}, Title: {}, Year: {}, Genre: {}, Rating: {}, ReadCount: {}", b.author,
                         b.title, b.year, b.genre, b.rating, b.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

// Специализируем базовый formatter для flat_map
template <typename K, typename V, typename... Args>
struct formatter<flat_map<K, V, Args...>, char> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    // Метод format отвечает за сам вывод
    auto format(const flat_map<K, V, Args...> &m, format_context &ctx) const {
        auto out = ctx.out();

        // Начинаем вывод с фигурной скобки
        out = format_to(out, "{{\n");

        bool first = true;
        for (const auto &[key, value] : m) {
            if (!first) {
                out = format_to(out, ",\n");
            }
            // Форматируем ключ в кавычках и значение через двоеточие
            out = format_to(out, "\t\"{}\": {}", key, value);
            first = false;
        }

        // Закрываем скобку
        return format_to(out, "\n}}");
    }
};

// Специализируем базовый formatter для reference_wrapper
template <typename K>
struct formatter<reference_wrapper<K>> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    // Метод format отвечает за сам вывод
    auto format(const reference_wrapper<K> &m, format_context &ctx) const {
        return format_to(ctx.out(), "{}", m.get());
    }
};

// Специализируем базовый formatter для vector
template <typename K, typename Alloc>
struct formatter<vector<K, Alloc>> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    // Метод format отвечает за сам вывод
    auto format(const vector<K, Alloc> &m, format_context &ctx) const {
        auto out = ctx.out();

        // Начинаем вывод с фигурной скобки
        out = format_to(out, "{{\n");

        bool first = true;
        for (const auto &value : m) {
            if (!first) {
                out = format_to(out, ",\n");
            }
            // Форматируем ключ в кавычках и значение через двоеточие
            out = format_to(out, "\t\"{}\"", value);
            first = false;
        }

        // Закрываем скобку
        return format_to(out, "\n}}");
    }
};

}  // namespace std