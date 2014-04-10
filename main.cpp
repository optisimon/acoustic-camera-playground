//
// Simple acoustic camera data flow test
// Copyright(C) 2014 Simon Gustafsson (optisimon.com)
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <assert.h>
#include <queue>
#include <stack>
#include <ctype.h>
#include <math.h>
#include <limits>

const double speedOfSound = 343; //  m/s

template<class T>
T sqr(T val) { return val*val; }

struct Pos {
	double x;
	double y;
	double z;

	Pos(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {
		// no code
	}

	Pos() : x(0), y(0), z(0) {
		// no code
	}

	double dist(const Pos& other)
	{
		return sqrt( sqr(x-other.x) + sqr(y-other.y) + sqr(z-other.z));
	}

	Pos operator*(double s) { return Pos(x*s, y*s, z*s); }
};



struct Mic {
	Pos pos;
};

class FakeMicArray {
	double radius_;
	std::vector<Mic> mics_;
public:
	/** @param radius radius of mic ring in meter */
	explicit FakeMicArray(int numMics, double radius) : radius_(radius)
	{
		for (int i = 0; i < numMics; i++)
		{
			double alpha = 2 * M_PI * i / numMics;
			std::cout << "alpha=" << alpha*180/M_PI;
			Mic tmp;
			tmp.pos.x = cos(alpha);
			tmp.pos.y = sin(alpha);
			tmp.pos.z = 0;
			mics_.push_back(tmp);
		}
	}

	const std::vector<Mic>& getMics() const {
		return mics_;
	}
};


class FakePointSoundSource {
	Pos pos_;
	double amplitude_;
	double freq_;
	double phase_;
public:
	FakePointSoundSource(Pos pos, double amplitude, double freq, double phase)
	: pos_(pos),
	  amplitude_(amplitude),
	  freq_(freq),
	  phase_(phase)
	{
		// no code
	}

	std::vector<double> getSamples(Pos listenerPos, int samplerate, int numSamples)
	{
		double distance =  pos_.dist(listenerPos);

		double t_distance = distance / speedOfSound;

		std::vector<double> retval(numSamples);

		for (int i = 0; i < numSamples; i++)
		{
			double t_sample = i * 1.0 / samplerate;
			retval[i] = amplitude_ / distance * sin(2 * M_PI * freq_ * (t_sample + t_distance) + phase_);

			// TODO: is it the sound pressure we should use, and is that proportional to 1/distance...
		}

		return retval;
	}

};

int main(int argc, char* argv[])
{
	std::cout << "Starting acoustic camera simulator" << std::endl;

	// Get input geometry
	const int numMics = 48;
	const int samplerate = 96000;

	const int numSamples = 10000;

	FakeMicArray micArray(numMics, 0.25);

	std::vector<Mic> mics = micArray.getMics();


	std::vector<std::vector<double> > fakeSamples(numMics);


	// Fill our fake sample buffer with fake data.
	FakePointSoundSource source(Pos(0,0,10), 1, 1000, 0);

	for (size_t i = 0; i < numMics; i++)
	{
		fakeSamples[i] = source.getSamples(mics[i].pos, samplerate, numSamples);
	}

	std::cout << "Now we have filled our buffres with fake generated samples" << std::endl;


	// Dumbest most stupid way to sum up data...

	const double xmin = -5;
	const double ymin = -5;
	const double xmax = 5;
	const double ymax = 5;
	const double dx = 0.25;
	const double dy = 0.25;

	std::ofstream out("delme.m");
	out << "colormap(gray)\n";

	std::cout << "img=[";
	out << "img=[";
	for (double y = ymin; y < ymax; y+=dy)
	{
		for (double x = xmin; x < xmax; x+=dx)
		{
			if (x != xmin) {
				std::cout << ", ";
				out << ", ";
			}

			int min_samples_delay = std::numeric_limits<int>::max();
			std::vector<int> samples_delay(numMics);
			for (int i = 0; i < numMics; i++)
			{
				double t_delay = Pos(x, y, 10).dist(mics[i].pos) / speedOfSound;
				int num_samples_delay = t_delay * samplerate + 0.5;
				samples_delay[i] = num_samples_delay;

				if (min_samples_delay > num_samples_delay) {
					min_samples_delay = num_samples_delay;
				}
			}

			for (int i = 0; i < numMics; i++)
			{
				samples_delay[i] -= min_samples_delay;
			}

			int max_samples_delay = 0;
			for (int i = 0; i < numMics; i++)
			{
				if (samples_delay[i] > max_samples_delay) {
					max_samples_delay = samples_delay[i];
				}
			}
			//std::cout << "max_samples_delay=" << max_samples_delay << std::endl;

			std::vector<double> beamFormedSamples(numSamples - max_samples_delay - 1);
			for (int n = 0; n < beamFormedSamples.size(); n++)
			{
				double sum = 0;
				for (int mic = 0; mic < numMics; mic++)
				{
					sum += fakeSamples[mic][n + samples_delay[mic]];
				}

				beamFormedSamples[n] = sum;
			}

			double rms = 0;
			for (size_t n = 0; n < beamFormedSamples.size(); n++) {
				double windowing = 0.5 * (1 - cos(2 * M_PI * n / (beamFormedSamples.size() - 1)));
				rms += sqr(beamFormedSamples[n] * windowing);
			}

			rms = sqrt(rms / beamFormedSamples.size());

			std::cout << rms;
			out << rms;
		}
		std::cout << "\n";
		out << "\n";
	}
	std::cout << "];" << std::endl;
	out << "];\nimagesc(img)" << std::endl;

	return 0;
}
