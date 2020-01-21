//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#pragma once

#include "Transducer.hpp"

#include <vector>


class ITransducerArray {
public:
	virtual const std::vector<Transducer>& getTransducers() const = 0;
};
