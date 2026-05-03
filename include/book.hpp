#pragma once

#include <flat_map>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(std::string_view s) {
    // Ваш код здесь
    if (!s.compare("Fiction"))
        return Genre::Fiction;
    else if (!s.compare("NonFiction"))
        return Genre::NonFiction;
    else if (!s.compare("SciFi"))
        return Genre::SciFi;
    else if (!s.compare("Biography"))
        return Genre::Biography;
    else if (!s.compare("Mystery"))
        return Genre::Mystery;
    return Genre::Unknown;
}

constexpr std::string_view GenreToString(Genre g) {
    std::string_view genre_str;

    // clang-format off
    using bookdb::Genre;
    switch (g) {
        case Genre::Fiction:    genre_str = "Fiction"; break;
        case Genre::Mystery:    genre_str = "Mystery"; break;
        case Genre::NonFiction: genre_str = "NonFiction"; break;
        case Genre::SciFi:      genre_str = "SciFi"; break;
        case Genre::Biography:  genre_str = "Biography"; break;
        case Genre::Unknown:    genre_str = "Unknown"; break;
        default:
            throw std::logic_error{"Unsupported bookdb::Genre"};
        }
    // clang-format on
    return genre_str;
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string title;
    std::string_view author;

    int year;
    Genre genre;
    double rating;
    int read_count;

    constexpr Book(const std::string &t, std::string_view a, int y, Genre g, double r, int rc)
        : title(t), author(a), year(y), genre(g), rating(r), read_count(rc) {}

    constexpr Book(const std::string &t, std::string_view a, int y, std::string_view g, double r, int rc)
        : title(t), author(a), year(y), genre(GenreFromString(g)), rating(r), read_count(rc) {}
};

}  // namespace bookdb
