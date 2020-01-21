//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "FakePointSoundSource.hpp"

#include <gtest/gtest.h>


TEST(FakePointSoundSource, Amplitude)
{
    Pos pos(0, 0, 0);
	double amplitude = 1.0;
	double freq = 1.0;
	double phase = 0.0;
    double speedOfSound = 1.0;

    FakePointSoundSource dut(pos, amplitude, freq, phase, speedOfSound);
    EXPECT_NEAR(1.0, dut.getAmplitude(Pos(1, 0, 0)), 1e-10);
    EXPECT_NEAR(0.5, dut.getAmplitude(Pos(2, 0, 0)), 1e-10);
    EXPECT_NEAR(1.0/3, dut.getAmplitude(Pos(3, 0, 0)), 1e-10);
}

TEST(FakePointSoundSource, OneDistanceUnitAway)
{
    Pos pos(0, 0, 0);
	double amplitude = 1.0;
	double freq = 1.0;
	double phase = 0.0;
    double speedOfSound = 1.0;

    FakePointSoundSource dut(pos, amplitude, freq, phase, speedOfSound);

    Pos listenerPos(0.0, 0.0, 1.0);
    int samplerate = 4;
    int numSamples = 4;
    std::vector<double> samples = dut.getSamples(listenerPos, samplerate, numSamples);

    ASSERT_EQ(4, samples.size());
    EXPECT_NEAR(0.0, samples[0], 1e-10);
    EXPECT_NEAR(1.0, samples[1], 1e-10);
    EXPECT_NEAR(0.0, samples[2], 1e-10);
    EXPECT_NEAR(-1.0, samples[3], 1e-10);
}

TEST(FakePointSoundSource, OneDistanceUnitAway_PhaseShift90Deg_Amplitude10)
{
    Pos pos(0, 0, 0);
	double amplitude = 10.0;
	double freq = 1.0;
	double phase = M_PI / 2;
    double speedOfSound = 1.0;

    FakePointSoundSource dut(pos, amplitude, freq, phase, speedOfSound);

    Pos listenerPos(0.0, 0.0, 1.0);
    int samplerate = 4;
    int numSamples = 4;
    std::vector<double> samples = dut.getSamples(listenerPos, samplerate, numSamples);

    ASSERT_EQ(4, samples.size());
    EXPECT_NEAR(10.0, samples[0], 1e-10);
    EXPECT_NEAR(0.0, samples[1], 1e-10);
    EXPECT_NEAR(-10.0, samples[2], 1e-10);
    EXPECT_NEAR(0.0, samples[3], 1e-10);
}


TEST(FakePointSoundSource, TwoDistanceUnitsAway)
{
    Pos pos(0, 0, 0);
	double amplitude = 1.0;
	double freq = 1.0;
	double phase = 0.0;
    double speedOfSound = 1.0;

    FakePointSoundSource dut(pos, amplitude, freq, phase, speedOfSound);

    Pos listenerPos(0.0, 0.0, 2.0);
    int samplerate = 4;
    int numSamples = 4;
    std::vector<double> samples = dut.getSamples(listenerPos, samplerate, numSamples);

    ASSERT_EQ(4, samples.size());
    EXPECT_NEAR(0.0, samples[0], 1e-10);
    EXPECT_NEAR(0.5, samples[1], 1e-10);
    EXPECT_NEAR(0.0, samples[2], 1e-10);
    EXPECT_NEAR(-0.5, samples[3], 1e-10);
}
