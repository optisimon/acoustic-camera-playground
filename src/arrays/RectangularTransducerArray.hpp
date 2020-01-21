//
// Copyright(C) 2014,2020 Simon Gustafsson (optisimon.com)
//

#pragma once

#include "ITransducerArray.hpp"


class RectangularTransducerArray : public ITransducerArray {
	std::vector<Transducer> _transducers;
public:
	RectangularTransducerArray(int numTransducersX, double spacingX, int numTransducersY, double spacingY);

	const std::vector<Transducer>& getTransducers() const override;
};
