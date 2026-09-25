/**
 * @brief Testes da coonstrução gulosa aleatorizada do TSP.
 */

 #include "Construction.h"
 #include "Data.h"
 #include "Solution.h"

 #include <cmath>
 #include <iostream>
 #include <random>
 #include <string>

 namespace {

    bool check(bool condition, const std::string& description) {
        if (condition) {
            std::cout << "[OK] " << description << '\n';
            return true;
        }

        std::cerr << "[FALHA] " << description << '\n';
        return false;
    }
 }

 int main() {
    std::string instancePath = "instances/berlin52.tsp";
    
    Data data(2, instancePath.c_str());
    data.read();

    constexpr unsigned int seed = 42;
    std::mt19937 randomGenerator(seed);

    const Solution solution = constructSolution(data, randomGenerator);

    bool success = true;

    success &= check(isValidSolution(solution, data.getDimension()),
    "A construção produz uma solução válida"
    );

    success &= check(
        solution.sequence.size() == 
        static_cast<std::size_t>(data.getDimension() + 1),

        "A solução possui n + 1 posições"
        );

    success &= check(
        solution.sequence.front() == 1 &&
            solution.sequence.back() == 1,
        "A solução começa e termina na cidade 1"
    );

    const double recalculatedCost = calculateCost(solution, data);

    success &= check(
        std::abs(solution.cost - recalculatedCost) < 1e-9,
        "O custo armazenado corresponde ao custo recalculado"
    );

    /*
     * Duas construções com a mesma semente devem produzir exatamente
     * a mesma solução, facilitando testes e reprodução de resultados.
     */
    std::mt19937 repeatedGenerator(seed);

    const Solution repeatedSolution = constructSolution(data, repeatedGenerator);
     
    success &= check(
        solution.sequence == repeatedSolution.sequence,
        "A mesma semente reproduz a mesma solução"
    );

    /*
     * Executa várias construções para verificar se a validade não depende
     * de uma única sequência específica de decisões aleatórias.
     */
    bool everyGeneratedSolutionIsValid = true;

    for (unsigned int currentSeed = 0;
         currentSeed < 20;
         ++currentSeed
        ) {
        std::mt19937 generator(currentSeed);

        const Solution generatedSolution = constructSolution(data, generator);

        if (!isValidSolution(
                generatedSolution,
                data.getDimension()
            )) {
            everyGeneratedSolutionIsValid = false;
            break;
        }
    }

    success &= check(
        everyGeneratedSolutionIsValid,
        "A construção permanece válida com diferentes sementes"
    );

    if (!success) {
        std::cerr << "\nAlgum teste da construção falhou.\n";

        return 1;
    }

    std::cout << "\nTodos os testes da construção passaram.\n";

    return 0;
 }