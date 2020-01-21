//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "Pos.hpp"

#include <gtest/gtest.h>

TEST(Pos,dist)
{
    Pos p1(10, 20, 30);
    Pos p2(13, 24, 30);
    Pos p3(10, 20, 33);

    EXPECT_NEAR(5.0, p1.dist(p2), 1e-10);
    EXPECT_NEAR(3.0, p1.dist(p3), 1e-10);
}

TEST(Pos,scaling)
{
    Pos p1(10, 20, 30);

    Pos p2 = p1 * 0.01;

    EXPECT_NEAR(0.1, p2.x, 1e-10);
    EXPECT_NEAR(0.2, p2.y, 1e-10);
    EXPECT_NEAR(0.3, p2.z, 1e-10);
}
