#include "bulk_average.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <limits>

using bulk_average::BulkAverage;

TEST(BulkAverageTest, EmptyState) {
    BulkAverage avg;
    EXPECT_EQ(avg.mean(), 0);
    EXPECT_EQ(avg.remainder(), 0U);
    EXPECT_EQ(avg.count(), 0U);
    EXPECT_DOUBLE_EQ(avg.to_double(), 0.0);
}

TEST(BulkAverageTest, SingleElement) {
    BulkAverage avg;
    avg +=42;
    EXPECT_EQ(avg.mean(), 42);
    EXPECT_EQ(avg.remainder(), 0U);
    EXPECT_EQ(avg.count(), 1U);
}

TEST(BulkAverageTest, TwoElements) {
    BulkAverage avg;
    avg +=3;
    avg +=7;
    // sum = 10, count = 2, mean = 5, remainder = 0
    EXPECT_EQ(avg.mean(), 5);
    EXPECT_EQ(avg.remainder(), 0U);
}

TEST(BulkAverageTest, NonDivisible) {
    BulkAverage avg;
    avg +=1;
    avg +=2;
    avg +=3;
    avg +=4;
    // sum = 10, count = 4, mean = 2, remainder = 2
    EXPECT_EQ(avg.mean(), 2);
    EXPECT_EQ(avg.remainder(), 2U);
    EXPECT_DOUBLE_EQ(avg.to_double(), 2.5);
}

TEST(BulkAverageTest, NegativeValues) {
    BulkAverage avg;
    avg +=(-10);
    avg +=(-20);
    avg +=(-30);
    // sum = -60, count = 3, mean = -20, remainder = 0
    EXPECT_EQ(avg.mean(), -20);
    EXPECT_EQ(avg.remainder(), 0U);
}

TEST(BulkAverageTest, MixedValues) {
    BulkAverage avg;
    avg +=10;
    avg +=(-10);
    // sum = 0, count = 2, mean = 0, remainder = 0
    EXPECT_EQ(avg.mean(), 0);
    EXPECT_EQ(avg.remainder(), 0U);
}

TEST(BulkAverageTest, LargePositiveValues) {
    constexpr int64_t MAX_VAL = std::numeric_limits<int64_t>::max();
    BulkAverage avg;
    avg +=MAX_VAL;
    avg +=MAX_VAL;
    // sum = 2 * INT64_MAX, count = 2, mean = INT64_MAX, remainder = 0
    EXPECT_EQ(avg.mean(), MAX_VAL);
    EXPECT_EQ(avg.remainder(), 0U);
}

TEST(BulkAverageTest, LargeNegativeValues) {
    constexpr int64_t MIN_VAL = std::numeric_limits<int64_t>::min();
    BulkAverage avg;
    avg +=MIN_VAL;
    avg +=MIN_VAL;
    // sum = 2 * INT64_MIN, count = 2, mean = INT64_MIN, remainder = 0
    EXPECT_EQ(avg.mean(), MIN_VAL);
    EXPECT_EQ(avg.remainder(), 0U);
}

TEST(BulkAverageTest, Streaming) {
    BulkAverage avg;
    for (int64_t i = 1; i <= 100; ++i) {
        avg +=i;
    }
    // sum = 5050, count = 100, mean = 50, remainder = 50
    EXPECT_EQ(avg.mean(), 50);
    EXPECT_EQ(avg.remainder(), 50U);
    EXPECT_DOUBLE_EQ(avg.to_double(), 50.5);
}

TEST(BulkAverageTest, Reset) {
    BulkAverage avg;
    avg +=100;
    avg.reset();
    EXPECT_EQ(avg.mean(), 0);
    EXPECT_EQ(avg.remainder(), 0U);
    EXPECT_EQ(avg.count(), 0U);

    avg +=42;
    EXPECT_EQ(avg.mean(), 42);
}

TEST(BulkAverageTest, ZeroThenMax) {
    constexpr int64_t MAX_VAL = std::numeric_limits<int64_t>::max();
    BulkAverage avg;
    avg +=0;
    avg +=MAX_VAL;
    // sum = MAX_VAL, count = 2
    EXPECT_EQ(avg.count(), 2U);
    EXPECT_EQ(avg.mean(), MAX_VAL / 2);
    EXPECT_EQ(avg.remainder(), 1U);
}

TEST(BulkAverageTest, MaxAndMin) {
    constexpr int64_t MAX_VAL = std::numeric_limits<int64_t>::max();
    constexpr int64_t MIN_VAL = std::numeric_limits<int64_t>::min();
    BulkAverage avg;
    avg +=MAX_VAL;
    avg +=MIN_VAL;
    // sum = INT64_MAX + INT64_MIN = -1, count = 2
    EXPECT_EQ(avg.count(), 2U);
    EXPECT_DOUBLE_EQ(avg.to_double(), -0.5);
}
