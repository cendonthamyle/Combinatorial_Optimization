#include "Solution.h"
#include "Data.h"

#include <ostream>
#include <vector>

double calculateCost(
    const Solution& solution,
    const Data& data
) {
    double totalCost = 0.0;

    for (std::size_t i = 0; i + 1 < solution.sequence.size(); ++i) {
        const int currentCity = solution.sequence[i];
        const int nextCity = solution.sequence[i + 1];

        totalCost += data.getDistance(currentCity, nextCity);
    }

    return totalCost;
}

bool isValidSolution(
    const Solution& solution,
    int dimension
) {
    if (dimension <= 0) {
        return false;
    }

    if (solution.sequence.size() !=
        static_cast<std::size_t>(dimension + 1)) {
        return false;
    }

    if (solution.sequence.front() != 1 ||
        solution.sequence.back() != 1) {
        return false;
    }

    std::vector<int> occurrences(dimension + 1, 0);

    // A última posição repete a cidade 1 para fechar o ciclo.
    for (std::size_t i = 0; i + 1 < solution.sequence.size(); ++i) {
        const int city = solution.sequence[i];

        if (city < 1 || city > dimension) {
            return false;
        }

        ++occurrences[city];
    }

    for (int city = 1; city <= dimension; ++city) {
        if (occurrences[city] != 1) {
            return false;
        }
    }

    return true;
}

void printSolution(
    const Solution& solution,
    std::ostream& output
) {
    for (std::size_t i = 0; i < solution.sequence.size(); ++i) {
        output << solution.sequence[i];

        if (i + 1 < solution.sequence.size()) {
            output << " -> ";
        }
    }

    output << '\n';
    output << "Custo: " << solution.cost << '\n';
}