#include <algorithm>
#include <print>
#include <vector>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"
#include "formatters.hpp"

using namespace bookdb;

int main() {
    //
    // Ниже приведён пример работы `BookDatabase`.
    //
    //     - Обратите внимание, что в этой функции реализованы основные возможности, охватывающие как обязательные, так
    //     и опциональные требования,
    //       которые не обязательны к реализации для сдачи работы.
    //     - Не забудьте перед созданием коммита вызвать 'run_clang_format.sh' для форматирования кода
    //

    // Create a book database
    BookDatabase<std::vector<Book>> db;

    // Add some books
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
    db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
    std::print("Books: {}\n\n", db);

    // Sorts
    std::sort(db.begin(), db.end(), comp::LessByAuthor{});
    std::print("Books sorted by author: {}\n\n==================\n", db);

    std::sort(db.begin(), db.end(), comp::LessByPopularity{});
    std::print("Books sorted by popularity: {}\n\n==================\n", db);

    // Author histogram
    auto histogram = buildAuthorHistogramFlat(db);
    std::println("Author histogram: {}", histogram);

    // Ratings
    auto genreRatings = calculateGenreRatings(db.begin(), db.end());
    std::println("\n\nAverage ratings by genres: {}\n", genreRatings);

    auto avrRating = calculateAverageRating(db);
    std::println("Average books rating in library: {}\n", avrRating);

    auto sampleBooks = sampleRandomBooks(db, 3);
    std::println("Samples of books: {}\n", sampleBooks);

    // Top 3 books
    auto topBooks = getTopNBy(db, 3, comp::LessByRating{});
    std::println("\n\nTop 3 books by rating: {}\n", topBooks);

    // Filters
    auto filtered1 = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    std::println("\n\nBooks from the 20th century with rating ≥ 4.5: {}\n", filtered1);

    auto filtered2 = filterBooks(db.begin(), db.end(), any_of(YearBetween(1800, 1899), GenreIs(Genre::SciFi)));
    std::println("\n\nBooks from the 19th century OR genre is SciFi: {}\n", filtered2);

    auto orwellBookIt = std::find_if(db.begin(), db.end(), [](const auto &v) { return v.author == "George Orwell"; });
    if (orwellBookIt != db.end()) {
        std::println("\n\nTransparent lookup by authors. Found Orwell's book: {}\n", *orwellBookIt);
    }

    return 0;
}

/*

Books: BookDatabase (size = 10): Books:
- Author: George Orwell, Title: 1984, Year: 1949, Genre: SciFi, Rating: 4, ReadCount: 190
- Author: George Orwell, Title: Animal Farm, Year: 1945, Genre: Fiction, Rating: 4.4, ReadCount: 143
- Author: F. Scott Fitzgerald, Title: The Great Gatsby, Year: 1925, Genre: Fiction, Rating: 4.5, ReadCount: 120
- Author: Harper Lee, Title: To Kill a Mockingbird, Year: 1960, Genre: Fiction, Rating: 4.8, ReadCount: 156
- Author: Jane Austen, Title: Pride and Prejudice, Year: 1813, Genre: Fiction, Rating: 4.7, ReadCount: 178
- Author: J.D. Salinger, Title: The Catcher in the Rye, Year: 1951, Genre: Fiction, Rating: 4.3, ReadCount: 112
- Author: Aldous Huxley, Title: Brave New World, Year: 1932, Genre: SciFi, Rating: 4.5, ReadCount: 98
- Author: Charlotte Brontë, Title: Jane Eyre, Year: 1847, Genre: Fiction, Rating: 4.6, ReadCount: 110
- Author: J.R.R. Tolkien, Title: The Hobbit, Year: 1937, Genre: Fiction, Rating: 4.9, ReadCount: 203
- Author: William Golding, Title: Lord of the Flies, Year: 1954, Genre: Fiction, Rating: 4.2, ReadCount: 89
Authors:
- William Golding
- J.R.R. Tolkien
- Charlotte Brontë
- Aldous Huxley
- Jane Austen
- Harper Lee
- J.D. Salinger
- F. Scott Fitzgerald
- George Orwell

Books sorted by author: BookDatabase (size = 10): Books:
- Author: Aldous Huxley, Title: Brave New World, Year: 1932, Genre: SciFi, Rating: 4.5, ReadCount: 98
- Author: Charlotte Brontë, Title: Jane Eyre, Year: 1847, Genre: Fiction, Rating: 4.6, ReadCount: 110
- Author: F. Scott Fitzgerald, Title: The Great Gatsby, Year: 1925, Genre: Fiction, Rating: 4.5, ReadCount: 120
- Author: George Orwell, Title: 1984, Year: 1949, Genre: SciFi, Rating: 4, ReadCount: 190
- Author: George Orwell, Title: Animal Farm, Year: 1945, Genre: Fiction, Rating: 4.4, ReadCount: 143
- Author: Harper Lee, Title: To Kill a Mockingbird, Year: 1960, Genre: Fiction, Rating: 4.8, ReadCount: 156
- Author: J.D. Salinger, Title: The Catcher in the Rye, Year: 1951, Genre: Fiction, Rating: 4.3, ReadCount: 112
- Author: J.R.R. Tolkien, Title: The Hobbit, Year: 1937, Genre: Fiction, Rating: 4.9, ReadCount: 203
- Author: Jane Austen, Title: Pride and Prejudice, Year: 1813, Genre: Fiction, Rating: 4.7, ReadCount: 178
- Author: William Golding, Title: Lord of the Flies, Year: 1954, Genre: Fiction, Rating: 4.2, ReadCount: 89
Authors:
- William Golding
- J.R.R. Tolkien
- Charlotte Brontë
- Aldous Huxley
- Jane Austen
- Harper Lee
- J.D. Salinger
- F. Scott Fitzgerald
- George Orwell

==================
Books sorted by popularity: BookDatabase (size = 10): Books:
- Author: William Golding, Title: Lord of the Flies, Year: 1954, Genre: Fiction, Rating: 4.2, ReadCount: 89
- Author: Aldous Huxley, Title: Brave New World, Year: 1932, Genre: SciFi, Rating: 4.5, ReadCount: 98
- Author: Charlotte Brontë, Title: Jane Eyre, Year: 1847, Genre: Fiction, Rating: 4.6, ReadCount: 110
- Author: J.D. Salinger, Title: The Catcher in the Rye, Year: 1951, Genre: Fiction, Rating: 4.3, ReadCount: 112
- Author: F. Scott Fitzgerald, Title: The Great Gatsby, Year: 1925, Genre: Fiction, Rating: 4.5, ReadCount: 120
- Author: George Orwell, Title: Animal Farm, Year: 1945, Genre: Fiction, Rating: 4.4, ReadCount: 143
- Author: Harper Lee, Title: To Kill a Mockingbird, Year: 1960, Genre: Fiction, Rating: 4.8, ReadCount: 156
- Author: Jane Austen, Title: Pride and Prejudice, Year: 1813, Genre: Fiction, Rating: 4.7, ReadCount: 178
- Author: George Orwell, Title: 1984, Year: 1949, Genre: SciFi, Rating: 4, ReadCount: 190
- Author: J.R.R. Tolkien, Title: The Hobbit, Year: 1937, Genre: Fiction, Rating: 4.9, ReadCount: 203
Authors:
- William Golding
- J.R.R. Tolkien
- Charlotte Brontë
- Aldous Huxley
- Jane Austen
- Harper Lee
- J.D. Salinger
- F. Scott Fitzgerald
- George Orwell

==================
Author histogram: {
        "Aldous Huxley": 1,
        "Charlotte Brontë": 1,
        "F. Scott Fitzgerald": 1,
        "George Orwell": 2,
        "Harper Lee": 1,
        "J.D. Salinger": 1,
        "J.R.R. Tolkien": 1,
        "Jane Austen": 1,
        "William Golding": 1
}

Average ratings by genres: {
        "Fiction": 4.55,
        "SciFi": 4.25
}

Average books rating in library: 4.49

Samples of books: {
        "Author: William Golding, Title: Lord of the Flies, Year: 1954, Genre: Fiction, Rating: 4.2, ReadCount: 89",
        "Author: Aldous Huxley, Title: Brave New World, Year: 1932, Genre: SciFi, Rating: 4.5, ReadCount: 98",
        "Author: J.R.R. Tolkien, Title: The Hobbit, Year: 1937, Genre: Fiction, Rating: 4.9, ReadCount: 203"
}

Top 3 books by rating: {
        "Author: J.R.R. Tolkien, Title: The Hobbit, Year: 1937, Genre: Fiction, Rating: 4.9, ReadCount: 203",
        "Author: Harper Lee, Title: To Kill a Mockingbird, Year: 1960, Genre: Fiction, Rating: 4.8, ReadCount: 156",
        "Author: Jane Austen, Title: Pride and Prejudice, Year: 1813, Genre: Fiction, Rating: 4.7, ReadCount: 178"
}

Books from the 20th century with rating ≥ 4.5: {
        "Author: Harper Lee, Title: To Kill a Mockingbird, Year: 1960, Genre: Fiction, Rating: 4.8, ReadCount: 156",
        "Author: J.R.R. Tolkien, Title: The Hobbit, Year: 1937, Genre: Fiction, Rating: 4.9, ReadCount: 203"
}

Books from the 19th century OR genre is SciFi: {
        "Author: George Orwell, Title: 1984, Year: 1949, Genre: SciFi, Rating: 4, ReadCount: 190",
        "Author: Aldous Huxley, Title: Brave New World, Year: 1932, Genre: SciFi, Rating: 4.5, ReadCount: 98",
        "Author: Charlotte Brontë, Title: Jane Eyre, Year: 1847, Genre: Fiction, Rating: 4.6, ReadCount: 110",
        "Author: Jane Austen, Title: Pride and Prejudice, Year: 1813, Genre: Fiction, Rating: 4.7, ReadCount: 178"
}

Transparent lookup by authors. Found Orwell's book: Author: George Orwell, Title: 1984, Year: 1949, Genre: SciFi, Rating: 4, ReadCount: 190

*/