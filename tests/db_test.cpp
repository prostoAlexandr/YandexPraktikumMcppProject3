#include "book.hpp"
#include "book_database.hpp"
#include <gtest/gtest.h>
#include <string>
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
    for (auto val : bdb.GetAuthors())
    {
        std::print("Authors: {}", val); 
    }
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

TEST(BookDatabaseTest, IteratorsTest)
{
    BookDatabase bdb({{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                      {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                      {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}});

    // Non-const iterators
    for (auto& it : bdb)
    {
        ++it.read_count;
    }

    const auto& bdb_cref = bdb;
    std::vector<std::string> test1;
    // Implicitly const iterators
    for (auto cit = bdb_cref.begin(); cit != bdb_cref.end(); ++cit)
    {
        // Cannot modify here
        test1.push_back(cit->title);
    }

    std::vector<std::string> test2;
    for(auto cit = bdb.cbegin(); cit != bdb.cend(); ++cit)
    {
        test2.push_back(cit->title);
    }

    EXPECT_EQ(test1, test2);
}