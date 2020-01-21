//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "RenderSound.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <limits>


std::vector<std::complex<double> >
getPhasors(std::vector<Transducer> const & transducers, Pos const & listenerPos, double audioFrequency, double speedOfSound)
{
	std::vector<std::complex<double> > phasors(transducers.size());
	for (size_t i = 0; i < transducers.size(); i++)
	{
		double distance = listenerPos.dist(transducers[i].pos);
		double amplitude = 1.0 / sqr(distance);
		double phase = 2 * M_PI * distance * audioFrequency / speedOfSound;
		phasors[i].real(amplitude * cos(phase));
		phasors[i].imag(amplitude * sin(phase));
	}
	return phasors;
}

std::complex<double> sumPhasors(std::vector<std::complex<double> > phasors)
{
	std::complex<double> resultingPhasor = {};
	for (auto const & phasor : phasors)
	{
		resultingPhasor += phasor;
	}
	return resultingPhasor;
}

/** 
 * Sample grid (x and y) determined by xvals and yvals
 * @param img destination location to store measured values (allocated by caller).
*/
void renderSoundOnWall(
		const std::vector<double>& xvals,
		const std::vector<double>& yvals,
		double z,
		double audioFrequency,
		const std::vector<Transducer>& transducers,
        const double speedOfSound,
		double* img)
{
	const int w = xvals.size();

	for (size_t yind = 0; yind < yvals.size(); yind++)
	{
		double y = yvals[yind];
		for (size_t xind = 0; xind < xvals.size(); xind++)
		{
			double x = xvals[xind];

			Pos listenerPos(x, y, z);

			std::vector<std::complex<double> > phasors = getPhasors(transducers, listenerPos, audioFrequency, speedOfSound);
			std::complex<double> resultingPhasor = sumPhasors(phasors);

			double rms = 1.0 / sqrt(2.0) * sqrt( sqr(resultingPhasor.real()) + sqr(resultingPhasor.imag()) );
			img[yind * w + xind] = rms;
		}
	}
}


// This is so fast that we don't need any optimizations
void renderSoundPolarPattern(
	double z,
	double audioFrequency,
	const std::vector<Transducer>& transducers,
	const double speedOfSound,
	std::string const & title,
	std::string const & filename)
{
	// draw 6 concentric rings and lines at each 30 degree slot
	int const w = 801;
	cv::Point2i center(w/2, w/2);
	double r = w/2 - 1;
	cv::Mat img(w, w, CV_8UC3, cv::Scalar(255, 255, 255));

	cv::putText(img, title.c_str(), cv::Point2i(5, 20), /* font */ 0, /* scale */ 0.75, cv::Scalar(0, 0, 0), /* thickness */ 1);
             
	for (int i = 0; i < 6; i++)
	{
		cv::circle(img, center, r * (i+1) / 6, cv::Scalar(0, 0, 0), 0, cv::LINE_8);
	}

	// all dB values are zero or negative
	auto dBToRadii = [r](double db) { return std::max<double>( r + r / 30.0 * db, 0); };

	for (int i = 1; i < 6; i++)
	{
		double dB = -i * 5;
		std::ostringstream oss;
		oss << -i*5 << "dB";
		cv::putText(img,
			oss.str().c_str(),
			cv::Point2i(w/2 + 5, center.y - dBToRadii(dB)),
			/* font */ 0,
			/* scale */ 0.67,
			cv::Scalar(0, 0, 0),
			/* thickness */ 1.25
		);
	}

	// Draw lines toward center every 30 degrees
	for (int a = 0; a < 360; a += 30)
	{
		cv::line(
			img,
			center,
			cv::Point2i(round(r * std::cos(a * M_PI / 180) + center.x), round(r * std::sin(a * M_PI / 180) + center.y)),
			cv::Scalar( 0, 0, 0 ),
			/* thickness */ 1,
			cv::LINE_8
		);
	}

	std::vector<double> vals(20000);
	for (size_t i = 0; i < vals.size(); i++)
	{
		double angle = i * 360.0 / (vals.size() - 1); // -1 to actually get both 0 and 360 degrees
		double x = z * cos(angle * M_PI / 180.0);
		double y = z * sin(angle * M_PI / 180.0);
		Pos listenerPos(x, 0, y);

		std::vector<std::complex<double> > phasors = getPhasors(transducers, listenerPos, audioFrequency, speedOfSound);
		std::complex<double> resultingPhasor = sumPhasors(phasors);

		double rms = 1.0 / sqrt(2.0) * sqrt( sqr(resultingPhasor.real()) + sqr(resultingPhasor.imag()) );
		vals[i] = rms;
	}

	double maxval = *std::max_element(vals.begin(), vals.end());

	bool firstPoint = true;
	cv::Point2d last(0, 0);
	for (size_t i = 0; i < vals.size(); i++)
	{
		double angle = i * 360.0 / (vals.size() - 1); // -1 to actually get both 0 and 360 degrees
		double relativeVal = vals[i] / maxval; // range 0 .. 1
		double scaleddB = 10.0 * log10(relativeVal);
		double scaledVal = dBToRadii(scaleddB);

		double x = scaledVal * cos(angle * M_PI / 180) + center.x;
		double y = scaledVal * sin(angle * M_PI / 180) + center.y;

		cv::Point curr(x, y);
		if (firstPoint)
		{
			firstPoint = false;
		}
		else
		{
			cv::line(img, last, curr, cv::Scalar(255, 0, 0));
		}

		last = curr;
	}
	//cv::imshow("polar", img);
	//cv::waitKey(0);
	cv::imwrite(filename, img);
}
