#include "Data.h"
#include "Solution.h"

#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

bool check(bool condition, const std::string& description) {
    if (condition) {
        std::cout << "[OK] " << description << '\n';
        return true;
    }

    std::cerr << "[FALHA] " << description << '\n';
    return false;
}

int main() {
    std::string instancePath = "instances/berlin52.tsp";
    const char* path = instancePath.data();

    Data data(2, instancePath.c_str());
    data.read();

    Solution solution;

    solution.sequence.resize(data.getDimension());
    std::iota(
        solution.sequence.begin(),
        solution.sequence.end(),
        1
    );
    solution.sequence.push_back(1);

    solution.cost = calculateCost(solution, data);

    bool success = true;

    success &= check(
        isValidSolution(solution, data.getDimension()),
        "rota sequencial valida"
    );

    success &= check(
        solution.sequence.size() ==
            static_cast<std::size_t>(data.getDimension() + 1),
        "rota possui n + 1 posicoes"
    );

    success &= check(
        solution.cost > 0.0,
        "custo calculado e positivo"
    );

    Solution duplicatedCity = solution;
    duplicatedCity.sequence[2] = duplicatedCity.sequence[1];

    success &= check(
        !isValidSolution(duplicatedCity, data.getDimension()),
        "cidade duplicada e detectada"
    );

    Solution openRoute = solution;
    openRoute.sequence.back() = 2;
    success &= check(
        !isValidSolution(openRoute, data.getDimension()),
        "rota aberta e detectada"
    );

    Solution invalidCity = solution;
    invalidCity.sequence[1] = data.getDimension() + 1;

    success &= check(
        !isValidSolution(invalidCity, data.getDimension()),
        "cidade fora do intervalo e detectada"
    );

    std::ostringstream output;
    printSolution(solution, output);

    success &= check(
        output.str().find("1 -> 2 -> 3") != std::string::npos,
        "solucao pode ser exibida"
    );

    if (!success) {
        std::cerr << "\nAlgum teste de Solution falhou.\n";
        return 1;
    }

    std::cout << "\nTodos os testes de Solution passaram.\n";
    return 0;
}