//
// Copyright(C) 2014,2020 Simon Gustafsson (optisimon.com)
//

#include "arrays/RandomTransducerArray.hpp"

/** 
 * @param numTransducers number of randomly placed transducers
 * @param width dimension of transducer array in meter
 * @param height dimension of transducer array in meter */
RandomTransducerArray::RandomTransducerArray(int numTransducers, double width, double height)
{
    for (int i = 0; i < numTransducers; i++)
    {
        double x = drand48() * width - width/2;
        double y = drand48() * height - height/2;

//        std::cout << "x,y= (" << x << ", " << x << ")\n";
        Transducer tmp;
        tmp.pos.x = x;
        tmp.pos.y = y;
        tmp.pos.z = 0;
        _transducers.push_back(tmp);
    }
}

const std::vector<Transducer>& RandomTransducerArray::getTransducers() const
{
    return _transducers;
}