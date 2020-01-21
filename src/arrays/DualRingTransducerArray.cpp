//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "arrays/DualRingTransducerArray.hpp"


void DualRingTransducerArray::addRing(int numTransducers, double radius)
{
    for (int i = 0; i < numTransducers; i++)
    {
        double alpha = 2 * M_PI * i / numTransducers;
//			std::cout << "alpha=" << alpha*180/M_PI << "\n";
        Transducer tmp;
        tmp.pos.x = radius * cos(alpha);
        tmp.pos.y = radius * sin(alpha);
        tmp.pos.z = 0;
        _transducers.push_back(tmp);
    }
}

/** @param radius1, radius2 radius of mic ring in meter */
DualRingTransducerArray::DualRingTransducerArray(int numTransducers1, double radius1, int numTransducers2, double radius2)
{
    addRing(numTransducers1, radius1);
    addRing(numTransducers2, radius2);
}

const std::vector<Transducer>& DualRingTransducerArray::getTransducers() const
{
    return _transducers;
}
