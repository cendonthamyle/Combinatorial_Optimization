/**
 * @brief Implementação do Iterated Local Search.
 */

#include "ILS.h"
#include "Construction.h"
#include "Data.h"
#include "LocalSearch.h"
#include "Perturbation.h"
#include "Solution.h"

#include <cstddef>
#include <limits>
#include <stdexcept>

namespace {

constexpr double improvementEpsilon = 1e-9;

} 

IlsParameters createDefaultIlsParameters(std::size_t dimension) {
    constexpr std::size_t outerIterationCount = 50;
    constexpr std::size_t largeInstanceThreshold = 150;

    if (dimension == 0) {throw std::invalid_argument(
            "A dimensão deve ser positiva"
        );
    }

    const std::size_t ilsIterationCount =
        dimension >= largeInstanceThreshold ? dimension / 2 : dimension;

    return {
        outerIterationCount,
        ilsIterationCount
    };
}

Solution iteratedLocalSearch(
    const Data& data,
    std::mt19937& randomGenerator,
    const IlsParameters& parameters) {

    if (parameters.maximumOuterIterations == 0) {
        throw std::invalid_argument("O número de iterações externas deve ser positivo");
    }

    if (parameters.maximumIlsIterations == 0) {
        throw std::invalid_argument("O número de iterações ILS deve ser positivo");
    }

    Solution bestOverallSolution;
    bestOverallSolution.cost = std::numeric_limits<double>::infinity();

    for (std::size_t outerIteration = 0;
         outerIteration < parameters.maximumOuterIterations;
         ++outerIteration) {
        
        Solution currentSolution = constructSolution(data, randomGenerator);
        Solution bestIterationSolution = currentSolution;

        std::size_t iterationsWithoutImprovement = 0;

        while (iterationsWithoutImprovement <= parameters.maximumIlsIterations) {
            randomVariableNeighborhoodDescent(
                currentSolution,
                data,
                randomGenerator
            );

            if (currentSolution.cost < bestIterationSolution.cost - improvementEpsilon) {
                bestIterationSolution = currentSolution;

                iterationsWithoutImprovement = 0;
            }

            currentSolution =
                perturbSolution(bestIterationSolution, data, randomGenerator);

            ++iterationsWithoutImprovement;
        }

        if (bestIterationSolution.cost < bestOverallSolution.cost - improvementEpsilon) {
            bestOverallSolution = bestIterationSolution;
        }
    }

    if (!isValidSolution(bestOverallSolution, data.getDimension())) {
        throw std::runtime_error("O ILS não produziu uma solução válida");
    }

    return bestOverallSolution;
}