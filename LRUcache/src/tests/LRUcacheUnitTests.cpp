#include "../objects/LRUcache.h"
#include <gtest/gtest.h>

TEST(LRUCacheTest, ConstructorTest) {
    LRUCache<int, std::string> cache(3);
    EXPECT_EQ(cache.getSize(), 0);
}

TEST(LRUCacheTest, PutAndGetTest) {
    LRUCache<int, std::string> cache(3);
    cache.put(1, "One");
    cache.put(2, "Two");

    EXPECT_EQ(cache.get(1), "One");
    EXPECT_EQ(cache.get(2), "Two");
}


TEST(LRUCacheTest, UpdateExistingElementTest) {
    LRUCache<int, std::string> cache(3);
    cache.put(1, "One");
    cache.put(1, "Updated One");

    EXPECT_EQ(cache.get(1), "Updated One");
}


TEST(LRUCacheTest, RemoveLeastRecentlyUsedTest) {
    LRUCache<int, std::string> cache(2);
    cache.put(1, "One");
    cache.put(2, "Two");

    // Переполнение
    cache.put(3, "Three");

    EXPECT_THROW(cache.get(1), std::logic_error);
    EXPECT_EQ(cache.get(2), "Two");
    EXPECT_EQ(cache.get(3), "Three");
}


TEST(LRUCacheTest, AccessUpdatesUsageOrderTest) {
    LRUCache<int, std::string> cache(2);
    cache.put(1, "One");
    cache.put(2, "Two");


    EXPECT_EQ(cache.get(1), "One");

    cache.put(3, "Three");

    EXPECT_THROW(cache.get(2), std::logic_error);
    EXPECT_EQ(cache.get(1), "One");
    EXPECT_EQ(cache.get(3), "Three");
}

TEST(LRUCacheTest, GetFromEmptyCacheTest) {
    LRUCache<int, std::string> cache(3);

    EXPECT_THROW(cache.get(1), std::logic_error);
}

TEST(LRUCacheTest, LargeDatasetTest) {
    LRUCache<int, int> cache(100);

    for (int i = 0; i < 200; ++i) {
        cache.put(i, i * 10);
    }

    for (int i = 0; i < 100; ++i) {
        EXPECT_THROW(cache.get(i), std::logic_error);
    }

    for (int i = 100; i < 200; ++i) {
        EXPECT_EQ(cache.get(i), i * 10);
    }
}

TEST(LRUCacheTest, CyclicAccessTest) {
    LRUCache<int, int> cache(3);

    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    EXPECT_EQ(cache.get(1), 100);

    cache.put(4, 400);

    EXPECT_THROW(cache.get(2), std::logic_error);
    EXPECT_EQ(cache.get(1), 100);
    EXPECT_EQ(cache.get(3), 300);
    EXPECT_EQ(cache.get(4), 400);
}
