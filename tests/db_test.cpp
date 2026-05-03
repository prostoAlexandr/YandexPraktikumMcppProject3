#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "formatters.hpp"
#include "statsistics.hpp"
#include <algorithm>
#include <deque>
#include <gtest/gtest.h>
#include <list>
#include <string>
#include <string_view>
#include <vector>

using namespace bookdb;

TEST(BookDatabaseTest, ItinializerConstructorTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    EXPECT_EQ(bdb.GetBooks().size(), 3);
    EXPECT_EQ(bdb.GetAuthors().size(), 2);
    EXPECT_TRUE(bdb.GetAuthors().contains("George Orwell"));
    EXPECT_TRUE(bdb.GetAuthors().contains("F. Scott Fitzgerald"));
}

TEST(BookDatabaseTest, PushBackTest) {
    BookDatabase bdb;
    bdb.PushBack({"1984", "George Orwell", 1949, Genre::SciFi, 4., 190});
    Book b{"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120};
    bdb.PushBack(b);
    bdb.PushBack(std::move(b));

    EXPECT_EQ(bdb.GetBooks().size(), 3);
    EXPECT_EQ(bdb.GetAuthors().size(), 2);
    EXPECT_TRUE(bdb.GetAuthors().contains("George Orwell"));
    EXPECT_TRUE(bdb.GetAuthors().contains("F. Scott Fitzgerald"));
}

TEST(BookDatabaseTest, EmplaceBackTest) {
    BookDatabase bdb;
    bdb.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    bdb.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    bdb.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);

    EXPECT_EQ(bdb.GetBooks().size(), 3);
    EXPECT_EQ(bdb.GetAuthors().size(), 2);
    EXPECT_TRUE(bdb.GetAuthors().contains("George Orwell"));
    EXPECT_TRUE(bdb.GetAuthors().contains("F. Scott Fitzgerald"));
}

TEST(BookDatabaseTest, ListDbTest) {
    BookDatabase<std::list<Book>> bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190}});
    bdb.PushBack({"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143});
    bdb.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);

    EXPECT_EQ(bdb.GetBooks().size(), 3);
    EXPECT_EQ(bdb.GetAuthors().size(), 2);
    EXPECT_TRUE(bdb.GetAuthors().contains("George Orwell"));
    EXPECT_TRUE(bdb.GetAuthors().contains("F. Scott Fitzgerald"));
}

TEST(BookDatabaseTest, DequeDbTest) {
    BookDatabase<std::deque<Book>> bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190}});
    bdb.PushBack({"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143});
    bdb.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);

    EXPECT_EQ(bdb.GetBooks().size(), 3);
    EXPECT_EQ(bdb.GetAuthors().size(), 2);
    EXPECT_TRUE(bdb.GetAuthors().contains("George Orwell"));
    EXPECT_TRUE(bdb.GetAuthors().contains("F. Scott Fitzgerald"));
}

TEST(BookDatabaseTest, IteratorsTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    std::vector<std::string> test1;
    // Non-const iterators
    for (auto &it : bdb) {
        ++it.read_count;
        test1.push_back(it.title);
    }

    const auto &bdb_cref = bdb;
    std::vector<std::string> test2;
    // Implicitly const iterators
    for (auto cit = bdb_cref.begin(); cit != bdb_cref.end(); ++cit) {
        // Cannot modify here
        test2.push_back(cit->title);
    }

    EXPECT_EQ(test1, test2);
}

TEST(BookDatabaseTest, StringMemoryTest) {
    BookDatabase<std::list<Book>> bdb;
    {
        std::string GeorgeOrwell("George Orwell");
        bdb.PushBack({"Animal Farm", GeorgeOrwell.c_str(), 1945, Genre::Fiction, 4.4, 143});
        bdb.EmplaceBack("The Great Gatsby", std::string("F. Scott Fitzgerald"), 1925, Genre::Fiction, 4.5, 120);
        bdb.EmplaceBack("1984", std::string_view(GeorgeOrwell), 1949, Genre::SciFi, 4., 190);
    }

    EXPECT_EQ(bdb.GetBooks().size(), 3);
    EXPECT_EQ(bdb.GetAuthors().size(), 2);
    EXPECT_TRUE(bdb.GetAuthors().contains("George Orwell"));
    EXPECT_TRUE(bdb.GetAuthors().contains("F. Scott Fitzgerald"));
}

TEST(BookDatabaseTest, YearFilterTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    auto filtered = filterBooks(bdb.begin(), bdb.end(), YearBetween(1944, 1946));
    EXPECT_EQ(filtered.size(), 1);
}

TEST(BookDatabaseTest, RatingFilterTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    auto filtered = filterBooks(bdb.begin(), bdb.end(), RatingAbove(4.2));
    EXPECT_EQ(filtered.size(), 2);
}

TEST(BookDatabaseTest, GenreFilterTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    auto filtered = filterBooks(bdb.begin(), bdb.end(), GenreIs(Genre::SciFi));
    EXPECT_EQ(filtered.size(), 1);
}

TEST(BookDatabaseTest, AllofFilterTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Mystery, 4.8, 156},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    auto filtered1 = filterBooks(bdb.begin(), bdb.end(), all_of(YearBetween(1940, 2000)));
    EXPECT_EQ(filtered1.size(), 3);
    auto filtered2 = filterBooks(bdb.begin(), bdb.end(), all_of(YearBetween(1940, 2000), RatingAbove(4.2)));
    EXPECT_EQ(filtered2.size(), 2);
    auto filtered3 =
        filterBooks(bdb.begin(), bdb.end(), all_of(YearBetween(1940, 2000), RatingAbove(4.2), GenreIs(Genre::Fiction)));
    EXPECT_EQ(filtered3.size(), 1);
}

TEST(BookDatabaseTest, AnyofFilterTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Mystery, 4.8, 156},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    auto filtered1 = filterBooks(bdb.begin(), bdb.end(), any_of(YearBetween(1944, 1946)));
    EXPECT_EQ(filtered1.size(), 1);
    auto filtered2 = filterBooks(bdb.begin(), bdb.end(), any_of(YearBetween(1944, 1946), RatingAbove(4.7)));
    EXPECT_EQ(filtered2.size(), 2);
    auto filtered3 =
        filterBooks(bdb.begin(), bdb.end(), any_of(YearBetween(1944, 1946), RatingAbove(4.7), GenreIs(Genre::SciFi)));
    EXPECT_EQ(filtered3.size(), 3);
}

TEST(BookDatabaseTest, AuthorHistogramTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::SciFi, 4.8, 156},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    auto rating = buildAuthorHistogramFlat(bdb);
    EXPECT_EQ(rating.size(), 3);
    EXPECT_EQ(rating["George Orwell"], 2);
    EXPECT_EQ(rating["Harper Lee"], 1);
    EXPECT_EQ(rating["F. Scott Fitzgerald"], 1);
}

TEST(BookDatabaseTest, CalculateRatingsTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::SciFi, 4.8, 156},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    auto rating = calculateGenreRatings(bdb.begin(), bdb.end());
    EXPECT_EQ(rating.size(), 2);
    EXPECT_DOUBLE_EQ(rating[GenreToString(Genre::Fiction)], 4.45);
    EXPECT_DOUBLE_EQ(rating[GenreToString(Genre::SciFi)], 4.4);

    auto average_rating = calculateAverageRating(bdb);
    EXPECT_DOUBLE_EQ(average_rating, 4.425);
}

TEST(BookDatabaseTest, RandomBooksTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::SciFi, 4.8, 156},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    auto sample = sampleRandomBooks(bdb, 2);
    EXPECT_EQ(sample.size(), 2);

    // Boundaries check
    sample = sampleRandomBooks(bdb, 10);
    EXPECT_EQ(sample.size(), 4);
}

TEST(BookDatabaseTest, GetTopNByTest) {
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::SciFi, 4.8, 156},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    auto top_by_rating = getTopNBy(bdb, 1, comp::LessByRating{});
    EXPECT_EQ(top_by_rating.size(), 1);
    EXPECT_DOUBLE_EQ(top_by_rating[0].get().rating, 4.8);

    auto top_by_author = getTopNBy(bdb, 2, comp::LessByAuthor{});
    EXPECT_EQ(top_by_author.size(), 2);
    EXPECT_EQ(top_by_author[0].get().author, "Harper Lee");

    auto top_by_popularity = getTopNBy(bdb, 3, comp::LessByPopularity{});
    EXPECT_EQ(top_by_popularity.size(), 3);
    EXPECT_DOUBLE_EQ(top_by_popularity[0].get().read_count, 190);

    // Boundaries check
    top_by_popularity = getTopNBy(bdb, 10, comp::LessByPopularity{});
    EXPECT_EQ(top_by_popularity.size(), 4);
}