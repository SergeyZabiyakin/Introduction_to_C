#include <gtest/gtest.h>

#include "Percolation.h"
#include "PercolationStats.h"

TEST(PercolationStatsTests, Percolation10Trials1000)
{
    const size_t dimension = 10;
    const size_t trials = 500;
    PercolationStats stats(dimension, trials);

    const double precision = 0.01;
    EXPECT_NEAR(0.5909, stats.get_mean(), precision);
    EXPECT_NEAR(0.0740, stats.get_standard_deviation(), precision);
    EXPECT_NEAR(0.5893, stats.get_confidence_low(), precision);
    EXPECT_NEAR(0.5975, stats.get_confidence_high(), precision);
}

TEST(PercolationStatsTests, Percolation5Trials5000)
{
    const size_t dimension = 5;
    const size_t trials = 5000;
    PercolationStats stats(dimension, trials);

    const double precision = 0.01;
    EXPECT_NEAR(0.5929, stats.get_mean(), precision);
    EXPECT_NEAR(0.1030, stats.get_standard_deviation(), precision);
    EXPECT_NEAR(0.5908, stats.get_confidence_low(), precision);
    EXPECT_NEAR(0.5949, stats.get_confidence_high(), precision);
}

TEST(PercolationStatsTests, Percolation2Trials10k)
{
    const size_t dimension = 2;
    const size_t trials = 10'000;
    PercolationStats stats(dimension, trials);

    const double precision = 0.01;
    EXPECT_NEAR(0.6669, stats.get_mean(), precision);
    EXPECT_NEAR(0.1177, stats.get_standard_deviation(), precision);
    EXPECT_NEAR(0.6662, stats.get_confidence_low(), precision);
    EXPECT_NEAR(0.6676, stats.get_confidence_high(), precision);
}

TEST(PercolationTest, Percolation3x3)
{
    // 0 x x x
    // 1 x x x
    // 2 x x x
    const size_t dimension = 3;
    Percolation percolation(dimension);
    ASSERT_FALSE(percolation.is_open(0, 0));
    ASSERT_FALSE(percolation.has_percolation());

    // 0 o x x
    // 1 x x x
    // 2 x x x
    percolation.open(0, 0);
    ASSERT_TRUE(percolation.is_open(0, 0));
    ASSERT_FALSE(percolation.has_percolation());

    // 0 o x x
    // 1 o x x
    // 2 x x x
    percolation.open(1, 0);
    ASSERT_TRUE(percolation.is_open(1, 0));
    ASSERT_FALSE(percolation.has_percolation());

    // 0 o x x
    // 1 o x x
    // 2 x o x
    percolation.open(2, 1);
    ASSERT_TRUE(percolation.is_open(2, 1));
    ASSERT_FALSE(percolation.has_percolation());

    // 0 o x x
    // 1 o o x
    // 2 x o x
    percolation.open(1, 1);
    ASSERT_TRUE(percolation.is_open(1, 1));
    ASSERT_TRUE(percolation.has_percolation());
    ASSERT_EQ(4, percolation.get_numbet_of_open_cells());
}
