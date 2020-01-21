//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#pragma once

#include <cmath>

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
		return std::sqrt(sqr(x-other.x) + sqr(y-other.y) + sqr(z-other.z));
	}

	Pos operator*(double s) { return Pos(x*s, y*s, z*s); }
};
