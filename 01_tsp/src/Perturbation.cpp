#include "Perturbation.h"
#include "Data.h"
#include "Solution.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

Solution perturbSolution(const Solution& solution, const Data& data, std::mt19937& randomGenerator) {
    constexpr std::size_t minimumSegmentSize = 2;

    if (!isValidSolution(solution, data.getDimension())) {
        throw std::invalid_argument(
            "A perturbação exige uma solução válida"
        );
    }

    const std::size_t movableCityCount = solution.sequence.size() - 2;

    if (movableCityCount < 2 * minimumSegmentSize) {
        throw std::invalid_argument(
            "A solução não possui cidades suficientes para dois segmentos"
        );
    }

    const std::size_t kitMaximumSegmentSize =
        static_cast<std::size_t>(
            std::ceil(static_cast<double>(data.getDimension()) / 10.0)
        );

    const std::size_t maximumSegmentSize =
        std::min(
            std::max(minimumSegmentSize, kitMaximumSegmentSize), movableCityCount / 2
        );

    std::uniform_int_distribution<std::size_t>segmentSizeDistribution(
            minimumSegmentSize,
            maximumSegmentSize
        );

    const std::size_t firstSegmentSize = segmentSizeDistribution(randomGenerator);
    const std::size_t secondSegmentSize = segmentSizeDistribution(randomGenerator);

    const std::size_t availableGapSize =
        movableCityCount - firstSegmentSize - secondSegmentSize;

    std::uniform_int_distribution<std::size_t>
        firstGapDistribution(0, availableGapSize);

    const std::size_t gapBeforeFirstSegment = firstGapDistribution(randomGenerator);
    const std::size_t remainingGapSize = availableGapSize - gapBeforeFirstSegment;

    std::uniform_int_distribution<std::size_t> middleGapDistribution(0, remainingGapSize);

    const std::size_t gapBetweenSegments = middleGapDistribution(randomGenerator);

    const std::size_t firstSegmentStart = 1 + gapBeforeFirstSegment;
    const std::size_t firstSegmentEnd = firstSegmentStart + firstSegmentSize - 1;

    const std::size_t secondSegmentStart = firstSegmentEnd + 1 + gapBetweenSegments;
    const std::size_t secondSegmentEnd = secondSegmentStart + secondSegmentSize - 1;

    std::vector<int> perturbedSequence;
    perturbedSequence.reserve(solution.sequence.size());

    perturbedSequence.insert(
        perturbedSequence.end(),
        solution.sequence.begin(),
        solution.sequence.begin() + static_cast<std::ptrdiff_t>(firstSegmentStart)
    );

    perturbedSequence.insert(
        perturbedSequence.end(),
        solution.sequence.begin() + static_cast<std::ptrdiff_t>(secondSegmentStart),
        solution.sequence.begin() + static_cast<std::ptrdiff_t>(secondSegmentEnd + 1)
    );

    perturbedSequence.insert(
        perturbedSequence.end(),
        solution.sequence.begin() + static_cast<std::ptrdiff_t>(firstSegmentEnd + 1),
        solution.sequence.begin() + static_cast<std::ptrdiff_t>(secondSegmentStart)
    );

    perturbedSequence.insert(
        perturbedSequence.end(),
        solution.sequence.begin() + static_cast<std::ptrdiff_t>(firstSegmentStart),
        solution.sequence.begin() + static_cast<std::ptrdiff_t>(firstSegmentEnd + 1)
    );

    perturbedSequence.insert(
        perturbedSequence.end(),
        solution.sequence.begin() + static_cast<std::ptrdiff_t>(secondSegmentEnd + 1),
        solution.sequence.end()
    );

    Solution perturbedSolution;
    perturbedSolution.sequence = std::move(perturbedSequence);

    perturbedSolution.cost = calculateCost(perturbedSolution, data);

    if (!isValidSolution(perturbedSolution, data.getDimension())) {
        throw std::runtime_error("A perturbação produziu uma soluçãoo inválida");
    }

    return perturbedSolution;
}