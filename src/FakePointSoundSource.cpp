//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "FakePointSoundSource.hpp"

FakePointSoundSource::FakePointSoundSource(Pos const & pos, double amplitude, double freq, double phase, double speedOfSound)
: pos_(pos),
    amplitude_(amplitude),
    freq_(freq),
    phase_(phase),
    speedOfSound_(speedOfSound)
{
    // no code
}

std::vector<double> FakePointSoundSource::getSamples(Pos const & listenerPos, int samplerate, int numSamples) const
{
    double distance =  pos_.dist(listenerPos);

    double t_distance = distance / speedOfSound_;

    std::vector<double> retval(numSamples);

    const double amplitude = getAmplitude(listenerPos);

    for (int i = 0; i < numSamples; i++)
    {
        double t_sample = i * 1.0 / samplerate;
        retval[i] = amplitude * sin(2 * M_PI * freq_ * (t_sample + t_distance) + phase_);

        // TODO: is it the sound pressure we should use, which is proportional to 1/distance,
        //       or sound intensity, which is proportional to 1/(distance*distance)
    }

    return retval;
}

double FakePointSoundSource::getAmplitude(Pos const & listenerPos) const
{
    double distance = pos_.dist(listenerPos);
    return amplitude_ / (distance * distance);
}