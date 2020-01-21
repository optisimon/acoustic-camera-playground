//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#pragma once

#include "ITransducerArray.hpp"


class DualRingTransducerArray : public ITransducerArray {
	std::vector<Transducer> _transducers;
	void addRing(int numTransducers, double radius);
public:
	/** @param radius1, radius2 radius of mic ring in meter */
	explicit DualRingTransducerArray(int numTransducers1, double radius1, int numTransducers2, double radius2);

	const std::vector<Transducer>& getTransducers() const override;
};
