#ifndef PERTURBATION_H
#define PERTURBATION_H

#include "Solution.h"
#include <random>

class Data;

Solution perturbSolution(
    const Solution& solution,
    const Data& data,
    std::mt19937& randomGenerator
);

#endif