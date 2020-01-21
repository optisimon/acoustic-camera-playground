//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "arrays/SingleRingTransducerArray.hpp"

#include <gtest/gtest.h>


TEST(SingleRingTransducerArray,TwoTransducers)
{
    SingleRingTransducerArray dut(2, 1.5);

    std::vector<Transducer> d = dut.getTransducers();
    ASSERT_EQ(2, d.size());
    EXPECT_NEAR(1.5, d[0].pos.x, 1e-10);
    EXPECT_NEAR(0.0, d[0].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[0].pos.z, 1e-10);

    EXPECT_NEAR(-1.5, d[1].pos.x, 1e-10);
    EXPECT_NEAR(0.0, d[1].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[1].pos.z, 1e-10);
}

TEST(SingleRingTransducerArray,FourTransducers)
{
    SingleRingTransducerArray dut(4, 1.5);

    std::vector<Transducer> d = dut.getTransducers();
    ASSERT_EQ(4, d.size());
    EXPECT_NEAR(1.5, d[0].pos.x, 1e-10);
    EXPECT_NEAR(0.0, d[0].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[0].pos.z, 1e-10);

    EXPECT_NEAR(0.0, d[1].pos.x, 1e-10);
    EXPECT_NEAR(1.5, d[1].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[1].pos.z, 1e-10);

    EXPECT_NEAR(-1.5, d[2].pos.x, 1e-10);
    EXPECT_NEAR(0.0, d[2].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[2].pos.z, 1e-10);

    EXPECT_NEAR(0.0, d[3].pos.x, 1e-10);
    EXPECT_NEAR(-1.5, d[3].pos.y, 1e-10);
    EXPECT_NEAR(0.0, d[3].pos.z, 1e-10);
}
