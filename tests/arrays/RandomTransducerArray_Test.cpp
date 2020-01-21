//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "arrays/RandomTransducerArray.hpp"

#include <gtest/gtest.h>


TEST(RandomTransducerArray,withinBounds)
{
    int numTransducers = 100;
    double width = 1.0;
    double height = 2.0;
    RandomTransducerArray dut(numTransducers, width, height);

    std::vector<Transducer> d = dut.getTransducers();
    ASSERT_EQ(100, d.size());
    
    for (Transducer const & t : d)
    {
        EXPECT_GE(t.pos.x, -0.5);
        EXPECT_LE(t.pos.x, 0.5);

        EXPECT_GE(t.pos.y, -1.0);
        EXPECT_LE(t.pos.y, 1.0);
    }

}
