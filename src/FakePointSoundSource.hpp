//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#pragma once

#include "Pos.hpp"

#include <cmath>
#include <vector>


class FakePointSoundSource {
	Pos pos_;
	double amplitude_;
	double freq_;
	double phase_;
    double speedOfSound_;
public:
    /** @param phase a phase shift (in radians) */
	FakePointSoundSource(Pos const & pos, double amplitude, double freq, double phase, double speedOfSound);
	std::vector<double> getSamples(Pos const & listenerPos, int samplerate, int numSamples) const;

    double getAmplitude(Pos const & listenerPos) const;
};
