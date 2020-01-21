//
// Copyright(C) 2014,2020 Simon Gustafsson (optisimon.com)
//

#include "arrays/RectangularTransducerArray.hpp"

/** @param radius1, radius2 radius of mic ring in meter */
RectangularTransducerArray::RectangularTransducerArray(int numTransducersX, double spacingX, int numTransducersY, double spacingY)
{
    double xoffs = - 0.5*(numTransducersX - 1) * spacingX;
    double yoffs = - 0.5*(numTransducersY - 1) * spacingY;

    for (int y = 0; y < numTransducersY; y++)
    {
        for (int x = 0; x < numTransducersX; x++)
        {
            double X = x * spacingX + xoffs;
            double Y = y * spacingY + yoffs;

//            std::cout << "x,y= (" << X << ", " << Y << ")\n";
            Transducer tmp;
            tmp.pos.x = X;
            tmp.pos.y = Y;
            tmp.pos.z = 0;
            _transducers.push_back(tmp);
        }
    }
}

const std::vector<Transducer>& RectangularTransducerArray::getTransducers() const
{
    return _transducers;
}
