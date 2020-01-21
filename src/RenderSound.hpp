//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#pragma once

#include "Transducer.hpp"

#include <complex>
#include <string>
#include <vector>


std::vector<std::complex<double> >
getPhasors(std::vector<Transducer> const & transducers, Pos const & listenerPos, double audioFrequency, double speedOfSound);

std::complex<double> sumPhasors(std::vector<std::complex<double> > phasors);

/** First naive renderer. */
void renderSoundOnWall(
		const std::vector<double>& xvals,
		const std::vector<double>& yvals,
		double z,
		double audioFrequency,
		const std::vector<Transducer>& transducers,
        const double speedOfSound,
		double* img);

void renderSoundPolarPattern(
		double z,
		double audioFrequency,
		const std::vector<Transducer>& transducers,
        const double speedOfSound,
		std::string const & title,
		std::string const & filename);
