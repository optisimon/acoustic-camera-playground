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
#include <string.h>
#include <algorithm>

#include "ArgumentParser.h"

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

	double dist(const Pos& other) const
	{
		return sqrt( sqr(x-other.x) + sqr(y-other.y) + sqr(z-other.z));
	}

	Pos operator*(double s) { return Pos(x*s, y*s, z*s); }
};



struct Mic {
	Pos pos;
};

class IFakeMicArray {
public:
	virtual const std::vector<Mic>& getMics() const = 0;
};

class FakeMicArray : public IFakeMicArray {
	double radius_;
	std::vector<Mic> mics_;
public:
	/** @param radius radius of mic ring in meter */
	explicit FakeMicArray(int numMics, double radius) : radius_(radius)
	{
		for (int i = 0; i < numMics; i++)
		{
			double alpha = 2 * M_PI * i / numMics;
			std::cout << "alpha=" << alpha*180/M_PI << "\n";
			Mic tmp;
			tmp.pos.x = radius * cos(alpha);
			tmp.pos.y = radius * sin(alpha);
			tmp.pos.z = 0;
			mics_.push_back(tmp);
		}
	}

	virtual const std::vector<Mic>& getMics() const {
		return mics_;
	}
};

class DualRingFakeMicArray : public IFakeMicArray {
	std::vector<Mic> mics_;
	void addRing(int numMics, double radius)
	{
		for (int i = 0; i < numMics; i++)
		{
			double alpha = 2 * M_PI * i / numMics;
			std::cout << "alpha=" << alpha*180/M_PI << "\n";
			Mic tmp;
			tmp.pos.x = radius * cos(alpha);
			tmp.pos.y = radius * sin(alpha);
			tmp.pos.z = 0;
			mics_.push_back(tmp);
		}
	}
public:
	/** @param radius1, radius2 radius of mic ring in meter */
	explicit DualRingFakeMicArray(int numMics1, double radius1, int numMics2, double radius2)
	{
		addRing(numMics1, radius1);
		addRing(numMics2, radius2);
	}

	virtual const std::vector<Mic>& getMics() const {
		return mics_;
	}
};

class RectangularFakeMicArray : public IFakeMicArray {
	std::vector<Mic> mics_;
public:
	/** @param radius1, radius2 radius of mic ring in meter */
	RectangularFakeMicArray(int numMicsX, double spacingX, int numMicsY, double spacingY)
	{
		double xoffs = - 0.5*(numMicsX-1) * spacingX;
		double yoffs = - 0.5*(numMicsY-1) * spacingY;

		for (int y = 0; y < numMicsY; y++) {
			for (int x = 0; x < numMicsX; x++) {
				double X = x * spacingX + xoffs;
				double Y = y * spacingY + yoffs;

				std::cout << "x,y= (" << X << ", " << Y << ")\n";
				Mic tmp;
				tmp.pos.x = X;
				tmp.pos.y = Y;
				tmp.pos.z = 0;
				mics_.push_back(tmp);
			}
		}
	}

	virtual const std::vector<Mic>& getMics() const {
		return mics_;
	}
};

class SpiralMicArray : public IFakeMicArray {
	std::vector<Mic> mics_;
	public:
		/** @param radius1, radius2 radius of mic ring in meter */
		SpiralMicArray(double a, double b, double c, double micsPerRevolution, int numMics)
		{
			for (int i = 0; i < numMics; i++)
			{
				double alpha = 2 * M_PI * i / micsPerRevolution;
				double x = a * cos(alpha) * exp(b*alpha) * (c * (1 + alpha));
				double y = a * sin(alpha) * exp(b*alpha) * (c * (1 + alpha));

				std::cout << "x,y= (" << x << ", " << y << "), r=" << sqrt(sqr(x)+sqr(y)) << "\n";
				Mic tmp;
				tmp.pos.x = x;
				tmp.pos.y = y;
				tmp.pos.z = 0;
				mics_.push_back(tmp);
			}
		}

		virtual const std::vector<Mic>& getMics() const {
			return mics_;
		}
};

class RandomFakeMicArray : public IFakeMicArray {
	std::vector<Mic> mics_;
public:
	/** @param radius1, radius2 radius of mic ring in meter */
	RandomFakeMicArray(int numMics, double width, double height)
	{
		for (int i = 0; i < numMics; i++)
		{
			double x = drand48() * width - width/2;
			double y = drand48() * height - height/2;

			std::cout << "x,y= (" << x << ", " << x << ")\n";
			Mic tmp;
			tmp.pos.x = x;
			tmp.pos.y = y;
			tmp.pos.z = 0;
			mics_.push_back(tmp);
		}
	}

	virtual const std::vector<Mic>& getMics() const {
		return mics_;
	}
};


class HomogeneousFakeMicArray : public IFakeMicArray {
	std::vector<Mic> mics_;
	void addMic(double X, double Y)
	{
		std::cout << "x,y= (" << X << ", " << Y << ")\n";
		Mic tmp;
		tmp.pos.x = X;
		tmp.pos.y = Y;
		tmp.pos.z = 0;
		mics_.push_back(tmp);
	}

	void addRectangular(int numMicsX, double spacingX, int numMicsY, double spacingY)
	{
		double xoffs = - 0.5*(numMicsX-1) * spacingX;
		double yoffs = - 0.5*(numMicsY-1) * spacingY;

		for (int y = 0; y < numMicsY; y++) {
			for (int x = 0; x < numMicsX; x++) {
				double X = x * spacingX + xoffs;
				double Y = y * spacingY + yoffs;

				std::cout << "x,y= (" << X << ", " << Y << ")\n";
				Mic tmp;
				tmp.pos.x = X;
				tmp.pos.y = Y;
				tmp.pos.z = 0;
				mics_.push_back(tmp);
			}
		}
	}
	struct Comp {
		bool operator()(const Mic&a, const Mic&b)
		{
			double ra = a.pos.dist(Pos(0,0,0));
			double rb = b.pos.dist(Pos(0,0,0));

			return ra < rb;
		}
	};
public:
	/** @param radius1, radius2 radius of mic ring in meter */
	HomogeneousFakeMicArray(int numMics, double spacing)
	{
		// Over-populate our array of mics
		addRectangular(numMics*2, spacing, numMics*2, spacing);

		// Sort them on radius
		Comp cmp;
		std::sort(mics_.begin(), mics_.end(), cmp);

		// remove the left-overs
		mics_.erase(mics_.begin() + numMics, mics_.end());
	}

	virtual const std::vector<Mic>& getMics() const {
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

/** First naive renderer. */
void renderSound(
		const std::vector<double>& xvals,
		const std::vector<double>& yvals,
		double z,
		const std::vector<std::vector<double> >& fakeSamples,
		const std::vector<Mic>& mics,
		int samplerate,
		double* img,
		int w,
		int h)
{
	const int numMics = mics.size();
	const int numSamples = fakeSamples[0].size();

	//out << "img=[";
	for (size_t yind = 0; yind < yvals.size(); yind++)
	{
		std::cout << "row " << yind << " of " << yvals.size() << std::endl;
		double y = yvals[yind];
		for (size_t xind = 0; xind < xvals.size(); xind++)
		{
			double x = xvals[xind];

			if (xind) {
				//std::cout << ", ";
				//out << ", ";
			}

			int min_samples_delay = std::numeric_limits<int>::max();
			std::vector<int> samples_delay(numMics);
			for (int i = 0; i < numMics; i++)
			{
				double t_delay = Pos(x, y, z).dist(mics[i].pos) / speedOfSound;
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

			//std::cout << rms;
			//out << rms;

			img[yind*w + xind] = rms;
		}
		//std::cout << "\n";
		//out << "\n";
	}
}


void renderSound_faster(
		const std::vector<double>& xvals,
		const std::vector<double>& yvals,
		double z,
		const std::vector<std::vector<double> >& fakeSamples,
		const std::vector<Mic>& mics,
		int samplerate,
		double* img,
		int w,
		int h)
{
	const int numMics = mics.size();
	const int numSamples = fakeSamples[0].size();
	std::vector<double> fakeSamples2(fakeSamples.size() * fakeSamples[0].size());
	// store data interleaved, so
	// mic0_t0, mic1_t0, mic2_t0, ... , micN_t0,    mic0_t1, mic1_t1, ...
	{
		int i = 0;

		for (int n = 0; n < fakeSamples[0].size(); n++)
		{
			for (int m = 0; m < fakeSamples.size(); m++)
			{
				fakeSamples2[i++] = fakeSamples[m][n];
			}
		}
	}


	//out << "img=[";
	for (size_t yind = 0; yind < yvals.size(); yind++)
	{
		std::cout << "row " << yind << " of " << yvals.size() << std::endl;
		double y = yvals[yind];
		for (size_t xind = 0; xind < xvals.size(); xind++)
		{
			double x = xvals[xind];

			if (xind) {
				//std::cout << ", ";
				//out << ", ";
			}

			int min_samples_delay = std::numeric_limits<int>::max();
			std::vector<int> samples_delay(numMics);
			for (int i = 0; i < numMics; i++)
			{
				double t_delay = Pos(x, y, z).dist(mics[i].pos) / speedOfSound;
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

			std::vector<double> beamFormedSamples(numSamples - max_samples_delay - 1, 0);
			const int limit = beamFormedSamples.size();
			for (int n = 0; n < limit; n++)
			{
				double sum = 0;
				for (int mic = 0; mic < (numMics); mic++)
				{
					//sum += fakeSamples[mic][n + samples_delay[mic]];
					sum += fakeSamples2[(n + samples_delay[mic])*numMics + mic];
				}

				beamFormedSamples[n] = sum;
			}

			double rms = 0;
			for (size_t n = 0; n < beamFormedSamples.size(); n++) {
				double windowing = 0.5 * (1 - cos(2 * M_PI * n / (beamFormedSamples.size() - 1)));
				rms += sqr(beamFormedSamples[n] * windowing);
			}

			rms = sqrt(rms / beamFormedSamples.size());

			//std::cout << rms;
			//out << rms;

			img[yind*w + xind] = rms;
		}
		//std::cout << "\n";
		//out << "\n";
	}
}



std::vector<double> linspace(double first, double last, int N)
{
	std::vector<double> retval(N);

	for (int i = 0; i < N; i++)
	{
		double alpha = i * 1.0 / (N-1);
		retval[i] = (1-alpha) * first  +  alpha * last;
	}

	return retval;
}


void drawMicsToFile(const char* filename, std::vector<Mic>& mics, int w, int h)
{
	unsigned char img[w*h];
	memset(img, 0, w*h);

	if (mics.size())
	{
		double maxx = 0.5;
		double minx = -0.5;
		double maxy = 0.5;
		double miny = -0.5;
		for (size_t i = 0; i < mics.size(); i++)
		{
			if (mics[i].pos.x > maxx) { maxx = mics[i].pos.x; }
			if (mics[i].pos.x < minx) { minx = mics[i].pos.x; }
			if (mics[i].pos.y > maxy) { maxy = mics[i].pos.y; }
			if (mics[i].pos.y < miny) { miny = mics[i].pos.y; }
		}

		//draw 1x1 meter square
		int x1 = (-0.5 - minx) / (maxx - minx) * (w-1);
		int x2 = ( 0.5 - minx) / (maxx - minx) * (w-1);
		int y1 = (-0.5 - miny) / (maxy - miny) * (h-1);
		int y2 = ( 0.5 - miny) / (maxy - miny) * (h-1);

		for (int i = x1; i < x2; i++)
		{
			img[y1*w + i] = 128;
			img[y2*w + i] = 128;
		}

		for (int i = y1; i < y2; i++)
		{
			img[i*w + x1] = 128;
			img[i*w + x2] = 128;
		}

		for (size_t i = 0; i < mics.size(); i++)
		{
			int x = (mics[i].pos.x - minx) / (maxx - minx) * (w-1);
			int y = (mics[i].pos.y - miny) / (maxy - miny) * (h-1);

			img[y*w + x] = 255;
		}

		std::ofstream imgFile(filename);
		imgFile << "P5\n" << w << " " << h << "\n" << 255 << "\n";

		for (int i = 0; i < w*h; i++) {
			imgFile << img[i];
		}
	}
}



int main(int argc, char* argv[])
{
	// Get input geometry
	int samplerate = 96000;
	int numSamples = 2048/4;
	int audioFrequency = 2000;

	int typeArg = 0;
	int showHelp = 0;
	int dimensionArg = 512;

	ArgumentParser parser;
	parser.addInt("--samplerate", &samplerate, "Mic sample rate");
	parser.addInt("-n", &numSamples, "Number of samples to generate in the simulator");
	parser.addInt("-f", &audioFrequency, "Frequency generated by simulator");
	parser.addInt("-t", &typeArg, "Type of mic array");
	parser.addInt("--dimension", &dimensionArg, "Width as well as height of result image");
	parser.addSwitch("-h", &showHelp, "Show this help");
	parser.parse(argc, argv);

	if (showHelp)
	{
		std::cout << parser.getHelp();
		return 1;
	}

	std::cout << "Starting acoustic camera simulator" << std::endl;

	enum MicArrayType {
		RING = 0,
		DOUBLE_RING = 1,
		RECTANGULAR = 2,
		HOMO = 3,
		SPIRAL1 = 4,
		RANDOM = 10,
	};

	MicArrayType type = MicArrayType( typeArg );

	IFakeMicArray* micArray = 0;

	switch(type)
	{
	case RING:
		micArray = new FakeMicArray(48, 0.25);
		break;
	case DOUBLE_RING:
		micArray = new DualRingFakeMicArray(32, 0.25, 16, 0.125);
		break;
	case RECTANGULAR:
		micArray = new RectangularFakeMicArray(7, 0.5/6, 7, 0.5/6);
		break;
	case HOMO:
		micArray = new HomogeneousFakeMicArray(52, 0.07);
		break;
	case SPIRAL1:
		micArray = new SpiralMicArray(0.010, 0.0, 0.3, 3.7, 48);
		break;
	case RANDOM:
	{
		long seed = time(NULL);
		std::cout << "Random seed = " << seed << std::endl;
		srand48(seed);
		micArray = new RandomFakeMicArray(48, 0.5, 0.5);
		break;
	}
	default:
		assert(false);
		return 1;
		break;
	}
	//FakeMicArray micArray(48, 0.25);

	//DualRingFakeMicArray micArray(32, 0.25, 32, 0.125);

	//RectangularFakeMicArray  micArray(8, 0.5/7, 8, 0.5/7);
	//RectangularFakeMicArray  micArray(5, 0.5/4, 5, 0.5/4);

	std::vector<Mic> mics = micArray->getMics();

	std::vector<std::vector<double> > fakeSamples(mics.size());


	// Fill our fake sample buffer with fake data.
	FakePointSoundSource source(Pos(0,0,10), 1, audioFrequency, 0);

	for (size_t i = 0; i < mics.size(); i++)
	{
		fakeSamples[i] = source.getSamples(mics[i].pos, samplerate, numSamples);
	}

	std::cout << "Now we have filled our buffers with fake generated samples" << std::endl;


	// Dumbest most stupid way to sum up data...

	const double z = 10;
	const double xmin = -10;
	const double ymin = -10;
	const double xmax = 10;
	const double ymax = 10;
	const int w = dimensionArg;
	const int h = dimensionArg;


	drawMicsToFile("delme_mics.pgm", mics, w, h);


	std::vector<double> xvals = linspace(xmin, xmax, w);
	std::vector<double> yvals = linspace(ymin, ymax, h);


	std::ofstream out("delme.m");
	out << "colormap(gray)\n";

	double img[w*h];

	renderSound_faster(xvals, yvals, z, fakeSamples, mics, samplerate, img, w, h);

	//std::cout << "];" << std::endl << std::endl;
	out << "];\nimagesc(img)" << std::endl;

	std::cout << "Done processing images" << std::endl;

	double max_val = 0;
	for (int i = 0; i < w*h; i++)
	{
		if (img[i] > max_val) {
			max_val = img[i];
		}
	}

	std::ofstream imgFile("delme.pgm");
	imgFile << "P5\n" << w << " " << h << "\n" << 255 << "\n";

	for (int i = 0; i < w*h; i++)
	{
		imgFile << (unsigned char)(img[i] * 255 / max_val);
	}


	return 0;
}
