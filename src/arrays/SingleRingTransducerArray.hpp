//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#pragma once

#include "ITransducerArray.hpp"


class SingleRingTransducerArray : public ITransducerArray {
	double radius_;
	std::vector<Transducer> _transducers;
public:
	/**
     * @param numTransducers number of transducers to fit 
     * @param radius radius of mic ring in meter */
	explicit SingleRingTransducerArray(int numTransducers, double radius);

	const std::vector<Transducer>& getTransducers() const override;
};