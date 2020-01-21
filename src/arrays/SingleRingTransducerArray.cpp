//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "arrays/SingleRingTransducerArray.hpp"

/** @param radius radius of mic ring in meter */
SingleRingTransducerArray::SingleRingTransducerArray(int numTransducers, double radius) : radius_(radius)
{
    for (int i = 0; i < numTransducers; i++)
    {
        double alpha = 2 * M_PI * i / numTransducers;
        // std::cout << "alpha=" << alpha*180/M_PI << "\n";
        Transducer tmp;
        tmp.pos.x = radius * cos(alpha);
        tmp.pos.y = radius * sin(alpha);
        tmp.pos.z = 0;
        _transducers.push_back(tmp);
    }
}

const std::vector<Transducer>& SingleRingTransducerArray::getTransducers() const
{
    return _transducers;
}
