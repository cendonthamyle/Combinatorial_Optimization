#ifndef ILS_H
#define ILS_H

#include "Solution.h"
#include <cstddef>
#include <random>

class Data;

struct IlsParameters {
    std::size_t maximumOuterIterations;
    std::size_t maximumIlsIterations;
};

IlsParameters createDefaultIlsParameters(
    std::size_t dimension
);

Solution iteratedLocalSearch(
    const Data& data,
    std::mt19937& randomGenerator,
    const IlsParameters& parameters
);

#endif