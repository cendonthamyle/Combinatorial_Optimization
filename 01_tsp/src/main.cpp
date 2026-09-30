 #include "Data.h"
 #include "ILS.h"
 #include "Solution.h"

 #include <chrono>
 #include <exception>
 #include <iostream>
 #include <numeric>
 #include <limits>
 #include <stdexcept>
 #include <string>

 namespace {
    constexpr unsigned int defaultSeed = 42;

    void printUsage(const char* executableName){
        std::cerr
            << "Uso: " << executableName
            << " <caminho-da-instancia.tsp> [semente]\n";
    }

    unsigned int parseSeed(const std::string& text) {
        std::size_t processedCharacterCount = 0;

        const unsigned long parsedValue = std::stoul(text, &processedCharacterCount);

        if (processedCharacterCount != text.size()) {
            throw std::invalid_argument("A semente deve conter somente algarismos");
        }

        if (parsedValue > std::numeric_limits<unsigned int>::max()) {
            throw std::out_of_range("A semente excede o limite permitido");
        }

        return static_cast<unsigned int>(parsedValue);
    }
 }

 int main(int argc, char* argv[]) {
    constexpr int minimumArgumentCount = 2;
    constexpr int maximumArgumentCount = 3;

    if (argc < minimumArgumentCount || argc > maximumArgumentCount) {
        printUsage(argv[0]);
        return 1;
    }

    try {
        Data data(argv[1]);
        data.read();

        const unsigned int seed = argc == maximumArgumentCount ? parseSeed(argv[2]) : defaultSeed;
        std::mt19937 randomGenerator(seed);

        const IlsParameters parameters =
            createDefaultIlsParameters(static_cast<std::size_t>(data.getDimension()));

        const auto startTime = std::chrono::steady_clock::now();

        const Solution bestSolution =
            iteratedLocalSearch(data, randomGenerator, parameters);

        const auto endTime = std::chrono::steady_clock::now();
        const std::chrono::duration<double>elapsedTime = endTime - startTime;


        if (!isValidSolution(bestSolution, data.getDimension())) {
            std::cerr << "Erro: O ILS produziu uma solução inválida.\n";
            return 1;
        }

        std::cout
            << "Instância: " << data.getInstanceName() << '\n'
            << "Dimensão: " << data.getDimension() << '\n'
            << "Semente: " << seed << '\n'
            << "maxIter: " << parameters.maximumOuterIterations << '\n'
            << "maxIterILS: " << parameters.maximumIlsIterations << '\n'
            << "Melhor custo: " << bestSolution.cost << '\n'
            << "Tempo: " << elapsedTime.count() << " segundos\n";

        printSolution(bestSolution, std::cout);

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Erro: " << error.what() << '\n';
        return 1;
    }
 }