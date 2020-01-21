//
// Copyright(C) 2014,2020 Simon Gustafsson (optisimon.com)
//

#pragma once

#include "ITransducerArray.hpp"


class RandomTransducerArray : public ITransducerArray {
	std::vector<Transducer> _transducers;
public:
	/** 
	 * @param numTransducers number of randomly placed transducers
	 * @param width dimension of transducer array in meter
	 * @param height dimension of transducer array in meter */
	RandomTransducerArray(int numTransducers, double width, double height);

	const std::vector<Transducer>& getTransducers() const override;
};
