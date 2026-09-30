#ifndef SOLUTION_H
#define SOLUTION_H

#include <iosfwd>
#include <vector>

class Data;

struct Solution {
    std::vector<int> sequence;
    double cost = 0.0;
};

double calculateCost(const Solution& solution, const Data& data);

bool isValidSolution(
    const Solution& solution,
    int dimension
);

void printSolution(
    const Solution& solution,
    std::ostream& output
);

#endif