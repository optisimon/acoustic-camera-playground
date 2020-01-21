//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "arrays/DualRingTransducerArray.hpp"

#include <gtest/gtest.h>

TEST(DualRingTransducerArray, twoAndFour)
{
    int numTransducers1 = 2;
    double radius1 = 1.5;
    int numTransducers2 = 4;
    double radius2 = 3.0;
    DualRingTransducerArray dut(numTransducers1, radius1, numTransducers2, radius2);
    std::vector<Transducer> d = dut.getTransducers();

    ASSERT_EQ(6, d.size());
    EXPECT_NEAR(1.5, d[0].pos.x, 1e-10);
    EXPECT_NEAR(0.0, d[0].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[0].pos.z, 1e-10);

    EXPECT_NEAR(-1.5, d[1].pos.x, 1e-10);
    EXPECT_NEAR(0.0, d[1].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[1].pos.z, 1e-10);

    EXPECT_NEAR(3.0, d[2].pos.x, 1e-10);
    EXPECT_NEAR(0.0, d[2].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[2].pos.z, 1e-10);

    EXPECT_NEAR(0.0, d[3].pos.x, 1e-10);
    EXPECT_NEAR(3.0, d[3].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[3].pos.z, 1e-10);

    EXPECT_NEAR(-3.0, d[4].pos.x, 1e-10);
    EXPECT_NEAR(0.0, d[4].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[4].pos.z, 1e-10);

    EXPECT_NEAR(0.0, d[5].pos.x, 1e-10);
    EXPECT_NEAR(-3.0, d[5].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[5].pos.z, 1e-10);
}
