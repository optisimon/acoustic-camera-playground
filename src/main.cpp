//
// Copyright(C) 2014,2020 Simon Gustafsson (optisimon.com)
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

#include "Pos.hpp"
#include "Transducer.hpp"
#include "ITransducerArray.hpp"
#include "FakePointSoundSource.hpp"
#include "RenderSound.hpp"

#include "arrays/SingleRingTransducerArray.hpp"
#include "arrays/DualRingTransducerArray.hpp"
#include "arrays/RectangularTransducerArray.hpp"
#include "arrays/RandomTransducerArray.hpp"

class SpiralTransducerArray : public ITransducerArray {
	std::vector<Transducer> _transducers;
	public:
		/** @param radius1, radius2 radius of mic ring in meter */
		SpiralTransducerArray(double a, double b, double c, double transducersPerRevolution, int numTransducers)
		{
			for (int i = 0; i < numTransducers; i++)
			{
				double alpha = 2 * M_PI * i / transducersPerRevolution;
				double x = a * cos(alpha) * exp(b*alpha) * (c * (1 + alpha));
				double y = a * sin(alpha) * exp(b*alpha) * (c * (1 + alpha));

//				std::cout << "x,y= (" << x << ", " << y << "), r=" << sqrt(sqr(x)+sqr(y)) << "\n";
				Transducer tmp;
				tmp.pos.x = x;
				tmp.pos.y = y;
				tmp.pos.z = 0;
				_transducers.push_back(tmp);
			}
		}

		const std::vector<Transducer>& getTransducers() const override {
			return _transducers;
		}
};

class HomogeneousTransducerArray : public ITransducerArray {
	std::vector<Transducer> _transducers;
	void addTransducer(double X, double Y)
	{
		std::cout << "x,y= (" << X << ", " << Y << ")\n";
		Transducer tmp;
		tmp.pos.x = X;
		tmp.pos.y = Y;
		tmp.pos.z = 0;
		_transducers.push_back(tmp);
	}

	void addRectangular(int numTransducersX, double spacingX, int numTransducersY, double spacingY)
	{
		double xoffs = - 0.5*(numTransducersX-1) * spacingX;
		double yoffs = - 0.5*(numTransducersY-1) * spacingY;

		for (int y = 0; y < numTransducersY; y++) {
			for (int x = 0; x < numTransducersX; x++) {
				double X = x * spacingX + xoffs;
				double Y = y * spacingY + yoffs;

//				std::cout << "x,y= (" << X << ", " << Y << ")\n";
				Transducer tmp;
				tmp.pos.x = X;
				tmp.pos.y = Y;
				tmp.pos.z = 0;
				_transducers.push_back(tmp);
			}
		}
	}
	struct Comp {
		bool operator()(const Transducer&a, const Transducer&b)
		{
			double ra = a.pos.dist(Pos(0,0,0));
			double rb = b.pos.dist(Pos(0,0,0));

			return ra < rb;
		}
	};
public:
	/**
	 * @param numTransducers number of transducers to place
	 * @param spacing how close to pack the elements */
	HomogeneousTransducerArray(int numTransducers, double spacing)
	{
		// Over-populate our array of mics
		addRectangular(numTransducers*2, spacing, numTransducers*2, spacing);

		// Sort them on radius
		Comp cmp;
		std::sort(_transducers.begin(), _transducers.end(), cmp);

		// remove the excess ones
		_transducers.erase(_transducers.begin() + numTransducers, _transducers.end());
	}

	const std::vector<Transducer>& getTransducers() const override {
		return _transducers;
	}
};



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


void drawMicsToFile(const char* filename, std::vector<Transducer>& mics, int w, int h)
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
	int audioFrequency = 2000;

	std::string outputFilename;

	int typeArg = 0;
	int showHelp = 0;
	int dimensionArg = 512;
	int polar = 0;

	ArgumentParser parser;
	parser.addInt("-f", &audioFrequency, "Frequency generated by simulator");
	parser.addInt("-t", &typeArg, "Type of mic array");
	parser.addInt("--dimension", &dimensionArg, "Width as well as height of wall image");
	parser.addString("-o", &outputFilename, "Destination image filename");
	parser.addSwitch("-h", &showHelp, "Show this help");
	parser.addSwitch("--polar", &polar, "Draw polar plot (instead of plot against plane in space)");
	parser.parse(argc, argv);

	if (showHelp)
	{
		std::cout << parser.getHelp();
		return 1;
	}

	if (!outputFilename.size())
	{
		std::cout << "ERROR: no output filename specified." << std::endl;
		return 2;
	}


	std::cout
	<< "Starting beamforming simulator: "
	<< ", f=" << audioFrequency
	<< ", t=" << typeArg
	<< ", dimension=" << dimensionArg
	<< ", o=" << outputFilename << std::endl;

	enum MicArrayType {
		RING = 0,
		DOUBLE_RING = 1,
		RECTANGULAR = 2,
		HOMO = 3,
		SPIRAL1 = 7,
		RESPEAKER_6_MIC_FOR_RPI = 6,
		RESPEAKER_4_MIC_FOR_RPI = 4,
		RANDOM = 10,
	};

	MicArrayType type = MicArrayType( typeArg );

	ITransducerArray* micArray = 0;

	switch(type)
	{
	case RING:
		micArray = new SingleRingTransducerArray(48, 0.25);
		break;
	case RESPEAKER_4_MIC_FOR_RPI:
		micArray = new RectangularTransducerArray(2, 0.058, 2, 0.058);
		break;
	case RESPEAKER_6_MIC_FOR_RPI:
		micArray = new SingleRingTransducerArray(7, 0.0925/2);
		break;
	case DOUBLE_RING:
		micArray = new DualRingTransducerArray(32, 0.25, 16, 0.125);
		break;
	case RECTANGULAR:
		micArray = new RectangularTransducerArray(7, 0.5/6, 7, 0.5/6);
		break;
	case HOMO:
		micArray = new HomogeneousTransducerArray(52, 0.07);
		break;
	case SPIRAL1:
		micArray = new SpiralTransducerArray(0.010, 0.0, 0.3, 3.7, 48);
		break;
	case RANDOM:
	{
		long seed = time(NULL);
		std::cout << "Random seed = " << seed << std::endl;
		srand48(seed);
		micArray = new RandomTransducerArray(48, 0.5, 0.5);
		break;
	}
	default:
		assert(false);
		return 1;
		break;
	}
	//SingleRingTransducerArray micArray(48, 0.25);

	//DualRingTransducerArray micArray(32, 0.25, 32, 0.125);

	//RectangularTransducerArray  micArray(8, 0.5/7, 8, 0.5/7);
	//RectangularTransducerArray  micArray(5, 0.5/4, 5, 0.5/4);

	std::vector<Transducer> mics = micArray->getTransducers();


	// Dumbest most stupid way to sum up data...

	const double z = 10;
	const double xmin = -10;
	const double ymin = -10;
	const double xmax = 10;
	const double ymax = 10;
	const int w = dimensionArg;
	const int h = dimensionArg;


	//drawMicsToFile("delme_mics.pgm", mics, w, h);


	std::vector<double> xvals = linspace(xmin, xmax, w);
	std::vector<double> yvals = linspace(ymin, ymax, h);


//	std::ofstream out("delme.m");
//	out << "colormap(gray)\n";

	double img[w*h];


	if (polar)
	{
		std::ostringstream oss;
		oss << "f=" << audioFrequency << ", t=" << typeArg;

		renderSoundPolarPattern(z, audioFrequency, mics, speedOfSound, oss.str(), outputFilename);
	}
	else
	{
		renderSoundOnWall(xvals, yvals, z, audioFrequency, mics, speedOfSound, img);
		//	//std::cout << "];" << std::endl << std::endl;
		//	out << "];\nimagesc(img)" << std::endl;

		std::cout << "Done processing image" << std::endl;

		double max_val = 0;
		for (int i = 0; i < w*h; i++)
		{
			if (img[i] > max_val) {
				max_val = img[i];
			}
		}

		if (outputFilename.size())
		{
			std::ofstream imgFile(outputFilename);
			imgFile << "P5\n" << w << " " << h << "\n" << 255 << "\n";

			for (int i = 0; i < w*h; i++)
			{
				imgFile << (unsigned char)(img[i] * 255 / max_val);
			}
		}
	}

	return 0;
}
