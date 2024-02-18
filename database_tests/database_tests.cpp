#include <gtest/gtest.h>

#include "database.h"
#include "test_struct.h"

TEST(DatabaseTests, InsertAndDeleteTest)
{
    database<test> db("testing_database");
    EXPECT_EQ(0, db.GetObjects().size());
    test t;
    t.first = 14;
    t.second = 16;
    for (int i = 0; i < 7; i++)
        t.test_string[i] = 'a' + i;
    const auto idTest = db.AddObject(t);
    EXPECT_EQ(1, idTest);
    db.DeleteObject(idTest);
    EXPECT_EQ(0, db.GetObjects().size());
}

TEST(DatabaseTests, EditTest)
{
    database<test> db("testing_database");
    EXPECT_EQ(0, db.GetObjects().size());
    test t;
    t.first = 14;
    t.second = 16;
    for (int i = 0; i < 7; i++)
        t.test_string[i] = 'a' + i;
    const auto idTest = db.AddObject(t);
    EXPECT_EQ(1, idTest);
    t.first = 20;
    t.second = 19;
    for (int i = 0; i < 9; i++)
        t.test_string[i] = 'g' + i;
    db.EditObject(idTest, t);
    const auto o = db.GetObject(idTest);
    EXPECT_EQ(t.first, o.first);
    EXPECT_EQ(t.second, o.second);
    for (int i = 0; i < 9; i++)
        EXPECT_EQ('g' + i, o.test_string[i]);

    db.DeleteObject(idTest);
    EXPECT_EQ(0, db.GetObjects().size());
}

TEST(DatabaseTests, ReloadTest)
{
    database<test> db("testing_database");
    EXPECT_EQ(0, db.GetObjects().size());
    test t;
    t.first = 14;
    t.second = 16;
    for (int i = 0; i < 7; i++)
        t.test_string[i] = 'a' + i;
    const auto idTest = db.AddObject(t);
    EXPECT_EQ(1, idTest);

    t.first = 20;
    t.second = 19;
    for (int i = 0; i < 9; i++)
        t.test_string[i] = 'g' + i;

    db.ReloadFromFile();
    const auto newId = db.AddObject(t);
    EXPECT_EQ(2, newId);

    t = db.GetObject(newId);
    EXPECT_EQ(20, t.first);
    EXPECT_EQ(19, t.second);
    for (int i = 0; i < 9; i++)
        EXPECT_EQ('g' + i, t.test_string[i]);

    db.DeleteObject(1);
    db.DeleteObject(2);
}
