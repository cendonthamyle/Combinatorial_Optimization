#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include "Solution.h"
#include <random>

class Data;

 Solution constructSolution(
    const Data& data,
    std::mt19937& randomGenerator
 );

#endif