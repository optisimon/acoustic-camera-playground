//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "arrays/RectangularTransducerArray.hpp"

#include <gtest/gtest.h>

TEST(RectangularTransducerArray,X1Y2)
{
    int numTransducersX = 1;
    double spacingX = 3;
    int numTransducersY = 2;
    double spacingY = 4;
    RectangularTransducerArray dut(numTransducersX, spacingX, numTransducersY, spacingY);

    // Expectation:
    //     X(0,2)
    //
    //
    //
    //     X(0,-2)

    std::vector<Transducer> d = dut.getTransducers();
    ASSERT_EQ(2, d.size());
    EXPECT_NEAR(0.0, d[0].pos.x, 1e-10);
    EXPECT_NEAR(-2.0, d[0].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[0].pos.z, 1e-10);

    EXPECT_NEAR(0.0, d[1].pos.x, 1e-10);
    EXPECT_NEAR(2.0, d[1].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[1].pos.z, 1e-10);
}

TEST(RectangularTransducerArray,X2Y1)
{
    int numTransducersX = 2;
    double spacingX = 3;
    int numTransducersY = 1;
    double spacingY = 4;
    RectangularTransducerArray dut(numTransducersX, spacingX, numTransducersY, spacingY);

    // Expectation:
    // X(-1.5, 0)       X(1.5, 0)

    std::vector<Transducer> d = dut.getTransducers();
    ASSERT_EQ(2, d.size());
    EXPECT_NEAR(-1.5, d[0].pos.x, 1e-10);
    EXPECT_NEAR(0.0, d[0].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[0].pos.z, 1e-10);

    EXPECT_NEAR(1.5, d[1].pos.x, 1e-10);
    EXPECT_NEAR(0.0, d[1].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[1].pos.z, 1e-10);
}
