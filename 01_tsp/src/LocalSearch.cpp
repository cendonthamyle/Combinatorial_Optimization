#include "LocalSearch.h"

#include "Data.h"
#include "Solution.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>
#include <stdexcept>
#include <random>

namespace {

constexpr double improvementEpsilon = 1e-9;

enum class Neighborhood {
    swap,
    twoOpt,
    reinsertion,
    orOpt2,
    orOpt3
};

std::vector<Neighborhood> createNeighborhoodList() {
    return {
        Neighborhood::swap,
        Neighborhood::twoOpt,
        Neighborhood::reinsertion,
        Neighborhood::orOpt2,
        Neighborhood::orOpt3
    };
}

void addAffectedEdge(
    std::vector<std::size_t>& edgePositions,
    std::size_t edgePosition,
    std::size_t edgeCount) {

        if (edgePosition >= edgeCount) {
            return;
        }

        const bool alreadyAdded =
            std::find(edgePositions.begin(),
                edgePositions.end(),
                edgePosition
            ) != edgePositions.end();

        if (!alreadyAdded) {
            edgePositions.push_back(edgePosition);
        }
    }

    double calculateSwapDelta(
        const Solution& solution,
        std::size_t firstPosition,
        std::size_t secondPosition,
        const Data& data) {

        const std::vector<int>& sequence = solution.sequence;
        const std::size_t edgeCount = sequence.size() - 1;

        std::vector<std::size_t> affectedEdges;
        affectedEdges.reserve(4);

        addAffectedEdge(affectedEdges, firstPosition - 1, edgeCount);
        addAffectedEdge(affectedEdges, firstPosition, edgeCount);
        addAffectedEdge(affectedEdges, secondPosition - 1, edgeCount);
        addAffectedEdge(affectedEdges,secondPosition,edgeCount);

        double currentAffectedCost = 0.0;
        double newAffectedCost = 0.0;

        const auto cityAfterSwap =
            [&](std::size_t position) -> int {
                if (position == firstPosition) {
                    return sequence[secondPosition];
                }

                if (position == secondPosition) {
                    return sequence[firstPosition];
                }

                return sequence[position];
            };

        for (const std::size_t edgePosition : affectedEdges) {
            const int currentFirstCity = sequence[edgePosition];

            const int currentSecondCity = sequence[edgePosition + 1];

            currentAffectedCost += 
                data.getDistance(
                currentFirstCity, 
                currentSecondCity
            );

            const int newFirstCity = cityAfterSwap(edgePosition);
            const int newSecondCity = cityAfterSwap(edgePosition + 1);

            newAffectedCost += data.getDistance(newFirstCity, newSecondCity);
        }

        return newAffectedCost - currentAffectedCost;
    }
}  

bool bestImprovementSwap(Solution& solution, const Data& data) {
    if (solution.sequence.size() < 4) {
        return false;
    }

    double bestDelta = 0.0;
    std::size_t bestFirstPosition = 0;
    std::size_t bestSecondPosition = 0;

    const std::size_t lastInternalPosition = solution.sequence.size() - 2;

    for (std::size_t firstPosition = 1;
         firstPosition < lastInternalPosition;
         ++firstPosition) {

        for (std::size_t secondPosition = firstPosition + 1;
             secondPosition <= lastInternalPosition;
             ++secondPosition) {
                
            const double delta = calculateSwapDelta(
                solution,
                firstPosition,
                secondPosition,
                data
            );

            if (delta < bestDelta - improvementEpsilon) {
                bestDelta = delta;
                bestFirstPosition = firstPosition;
                bestSecondPosition = secondPosition;
            }
        }
    }

    if (bestDelta >= -improvementEpsilon) {
        return false;
    }

    std::swap(
        solution.sequence[bestFirstPosition],
        solution.sequence[bestSecondPosition]
    );

    solution.cost += bestDelta;

    return true;
}

bool bestImprovement2Opt(Solution& solution, const Data& data) {
    if (solution.sequence.size() < 5) {
        return false;
    }

    double bestDelta = 0.0;
    std::size_t bestSegmentStart = 0;
    std::size_t bestSegmentEnd = 0;

    const std::size_t lastInternalPosition = solution.sequence.size() - 2;

    for (std::size_t segmentStart = 1;
        segmentStart < lastInternalPosition;
         ++segmentStart) {

        for (std::size_t segmentEnd = segmentStart + 1;
             segmentEnd <= lastInternalPosition;
             ++segmentEnd) {

            const int beforeSegment =solution.sequence[segmentStart - 1];

            const int firstCityInSegment = solution.sequence[segmentStart];
            const int lastCityInSegment = solution.sequence[segmentEnd];

            const int afterSegment = solution.sequence[segmentEnd + 1];

            const double removedCost =
                data.getDistance(
                    beforeSegment,
                    firstCityInSegment
                ) +
                data.getDistance(
                    lastCityInSegment,
                    afterSegment
                );

            const double addedCost =
                data.getDistance(
                    beforeSegment,
                    lastCityInSegment
                ) +
                data.getDistance(
                    firstCityInSegment,
                    afterSegment
                );

            const double delta = addedCost - removedCost;

            if (delta < bestDelta - improvementEpsilon) {
                bestDelta = delta;
                bestSegmentStart = segmentStart;
                bestSegmentEnd = segmentEnd;
            }
        }
    }

    if (bestDelta >= -improvementEpsilon) {
        return false;
    }

    std::reverse(
        solution.sequence.begin() +
            static_cast<std::ptrdiff_t>(bestSegmentStart),
        solution.sequence.begin() +
            static_cast<std::ptrdiff_t>(bestSegmentEnd + 1)
    );

    solution.cost += bestDelta;

    return true;
}

bool bestImprovementOrOpt(
    Solution& solution,
    const Data& data,
    std::size_t blockSize) {

    constexpr std::size_t minimumBlockSize = 1;
    constexpr std::size_t maximumBlockSize = 3;

    if (blockSize < minimumBlockSize ||
        blockSize > maximumBlockSize) {
        throw std::invalid_argument(
            "O tamanho do bloco Or-opt deve estar entre 1 e 3"
        );
    }

    
    const std::size_t movableCityCount =
        solution.sequence.size() >= 2
            ? solution.sequence.size() - 2
            : 0;

    if (blockSize > movableCityCount) {
        return false;
    }

    double bestDelta = 0.0;
    std::size_t bestBlockStart = 0;
    std::size_t bestInsertionEdge = 0;

    const std::size_t lastInternalPosition = solution.sequence.size() - 2;

    const std::size_t lastEdgePosition = solution.sequence.size() - 2;

    const std::size_t lastBlockStart = lastInternalPosition - blockSize + 1;

    for (std::size_t blockStart = 1;
         blockStart <= lastBlockStart;
         ++blockStart) {

        const std::size_t blockEnd = blockStart + blockSize - 1;

        const int previousCity = solution.sequence[blockStart - 1];
        const int firstCityInBlock = solution.sequence[blockStart];
        const int lastCityInBlock = solution.sequence[blockEnd];
        const int nextCity = solution.sequence[blockEnd + 1];

        for (std::size_t insertionEdge = 0;
             insertionEdge <= lastEdgePosition;
             ++insertionEdge) {
            
            const bool insertionTouchesBlock =
                insertionEdge >= blockStart - 1 &&
                insertionEdge <= blockEnd;

            if (insertionTouchesBlock) {
                continue;
            }

            const int insertionLeftCity = solution.sequence[insertionEdge];
            const int insertionRightCity = solution.sequence[insertionEdge + 1];

            const double removedCost =
                data.getDistance(
                    previousCity,
                    firstCityInBlock
                ) +
                data.getDistance(
                    lastCityInBlock,
                    nextCity
                ) +
                data.getDistance(
                    insertionLeftCity,
                    insertionRightCity
                );

            const double addedCost =
                data.getDistance(
                    previousCity,
                    nextCity
                ) +
                data.getDistance(
                    insertionLeftCity,
                    firstCityInBlock
                ) +
                data.getDistance(
                    lastCityInBlock,
                    insertionRightCity
                );

            const double delta = addedCost - removedCost;

            if (delta < bestDelta - improvementEpsilon) {
                bestDelta = delta;
                bestBlockStart = blockStart;
                bestInsertionEdge = insertionEdge;
            }
        }
    }

    if (bestDelta >= -improvementEpsilon) {
        return false;
    }

    const std::size_t bestBlockEnd = bestBlockStart + blockSize - 1;

    const std::vector<int> movedBlock(
        solution.sequence.begin() +
            static_cast<std::ptrdiff_t>(bestBlockStart),
        solution.sequence.begin() +
            static_cast<std::ptrdiff_t>(bestBlockEnd + 1)
    );

    solution.sequence.erase(
        solution.sequence.begin() +
            static_cast<std::ptrdiff_t>(bestBlockStart),
        solution.sequence.begin() +
            static_cast<std::ptrdiff_t>(bestBlockEnd + 1)
    );

    std::size_t insertionPosition = 0;

    if (bestInsertionEdge < bestBlockStart) {
        insertionPosition = bestInsertionEdge + 1;
    } else {
        insertionPosition =
            bestInsertionEdge - blockSize + 1;
    }

    solution.sequence.insert(
        solution.sequence.begin() +
            static_cast<std::ptrdiff_t>(insertionPosition),
        movedBlock.begin(),
        movedBlock.end()
    );

    solution.cost += bestDelta;

    return true;
}

void randomVariableNeighborhoodDescent(
    Solution& solution,
    const Data& data,
    std::mt19937& randomGenerator) {

    std::vector<Neighborhood> activeNeighborhoods = createNeighborhoodList();

    while (!activeNeighborhoods.empty()) {
        std::uniform_int_distribution<std::size_t>
            neighborhoodDistribution(
                0,
                activeNeighborhoods.size() - 1
            );

        const std::size_t selectedIndex = neighborhoodDistribution(randomGenerator);

        const Neighborhood selectedNeighborhood = activeNeighborhoods[selectedIndex];

        bool improved = false;

        switch (selectedNeighborhood) {
            case Neighborhood::swap:improved = bestImprovementSwap(solution, data);
                break;

            case Neighborhood::twoOpt:improved = bestImprovement2Opt(solution, data);
                break;

            case Neighborhood::reinsertion:
                improved = bestImprovementOrOpt(solution, data, 1);
                break;

            case Neighborhood::orOpt2:
                improved = bestImprovementOrOpt(solution, data, 2);
                break;

            case Neighborhood::orOpt3:
                improved = bestImprovementOrOpt(solution, data, 3);
                break;
        }

        if (improved) {
            activeNeighborhoods = createNeighborhoodList();
        } else {
            activeNeighborhoods.erase(
                activeNeighborhoods.begin() +
                    static_cast<std::ptrdiff_t>(
                        selectedIndex
                    )
            );
        }
    }
}