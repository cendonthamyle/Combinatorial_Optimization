 #include "Construction.h"
 #include "Data.h"
 #include "Solution.h"

 #include <algorithm>
 #include <cmath>
 #include <cstddef>
 #include <numeric>
 #include <random>
 #include <stdexcept>
 #include <vector>

 namespace {
    struct InsertionInfo {
        int insertedCity;
        std::size_t edgePosition;
        double delta;
    };
    
    /**
     * @brief Calcula todas as inserções possíveis na solução parcial.
     *
     * Para inserir uma cidade k entre duas cidades consecutivas i e j, remove-se
     * a aresta (i,j) e adicionam-se as arestas (i,k) e (k,j). O aumento de custo é:
     *
     *     delta = c(i,k) + c(k,j) - c(i,j)
     */
    std::vector<InsertionInfo> calculateInsertionOptions(
        const Solution& partialSolution,
        const std::vector<int>& candidateCities,
        const Data& data
    ) {
        std::vector<InsertionInfo> options;

        const std::size_t edgeCount = partialSolution.sequence.size() - 1;

        options.reserve(edgeCount * candidateCities.size());

        for (std::size_t edgePosition = 0; edgePosition < edgeCount; ++edgePosition) {
            const int firstCity = partialSolution.sequence[edgePosition];
            const int secondCity = partialSolution.sequence[edgePosition + 1];

            for ( const int candidateCity : candidateCities) {
                const double delta = 
                    data.getDistance(firstCity, candidateCity) +
                    data.getDistance(candidateCity, secondCity) -
                    data.getDistance(firstCity, secondCity);

                options.push_back({
                    candidateCity,
                    edgePosition,
                    delta
                });                    
            }
        }

        return options;
    }

    void removeCandidate(
        std::vector<int>& candidateCities, int insertedCity) {

        const auto position = std::find(
            candidateCities.begin(),
            candidateCities.end(),
            insertedCity
        );

        if (position == candidateCities.end()) {
            throw std::runtime_error(
                "A cidade inserida não foi encontrada na lista de candidatos"
            );
        }
        candidateCities.erase(position);
    }
 }

 Solution constructSolution(const Data& data, std::mt19937& randomGenerator) {
    constexpr int depot = 1;
    constexpr int initialRandomCityCount = 3;

    const int dimension = data.getDimension();

    if (dimension < depot + initialRandomCityCount) {
        throw std::invalid_argument(
            "A construção exige uma instância com pelo menos 4 cidades"
        );
    }

    std::vector<int> shuffledCities(static_cast<std::size_t>(dimension - 1));

    std::iota(shuffledCities.begin(), shuffledCities.end(), depot + 1);
    std::shuffle( shuffledCities.begin(), shuffledCities.end(), randomGenerator);

    Solution solution;
    solution.sequence.push_back(depot);

    for (int index = 0; index < initialRandomCityCount; ++index) {
        solution.sequence.push_back(shuffledCities[static_cast<std::size_t>(index)]);
    }

    solution.sequence.push_back(depot);

    std::vector<int> candidateCities(
        shuffledCities.begin() + initialRandomCityCount,
        shuffledCities.end()
    );

    std::uniform_real_distribution<double> alphaDistribution(0.0, 1.0);

    while (!candidateCities.empty()) {
        std::vector<InsertionInfo> options = 
            calculateInsertionOptions(solution, candidateCities, data);

        std::sort(
            options.begin(),
            options.end(),
            [](const InsertionInfo& first, const InsertionInfo& second) {
                return first.delta < second.delta;
            }
        );

        const double alpha = alphaDistribution(randomGenerator);

        std::size_t restrictedListSize =
            static_cast<std::size_t>(std::ceil(alpha * options.size()));

        restrictedListSize = std::max<std::size_t>(1, restrictedListSize);

        std::uniform_int_distribution<std::size_t> optionDistribution(0, restrictedListSize - 1);

        const InsertionInfo selectedOption = options[optionDistribution(randomGenerator)];

            solution.sequence.insert(
            solution.sequence.begin() +
                static_cast<std::ptrdiff_t>(selectedOption.edgePosition + 1),
            selectedOption.insertedCity
            );

        removeCandidate(
            candidateCities,
            selectedOption.insertedCity
        );
    }

    solution.cost= calculateCost(solution, data);

    if (!isValidSolution(solution, dimension)) {
        throw std::runtime_error(
            "A construção produziu uma solução inválida"
        );
    }

    return solution;
 }