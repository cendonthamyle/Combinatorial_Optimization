#include "Data.h"

#include <cmath>
#include <exception>
#include <iostream>
#include <string>

/* Esse programa testa quatro formatos de intância:
 * berlin52: distância euclidiana
 * att48: pseudo-euclidiana ATT
 * berma14: geográfica
 * bayg29: matriz explícita
 * 
 * Esse programa verifica: dimensão, nome, diagonal e simetria.
 */
bool checkCondition(bool condition, const std::string& description) {
    if (condition) {
        std::cout << "[OK] " << description << '\n';
        return true;
    }

    std::cerr << "[FALHA] " << description << '\n';
    return false;
}

bool testInstance(
    const std::string& path,
    int expectedDimension,
    const std::string& expectedName
) {
    std::cout << "\nTestando " << expectedName << "...\n";

    try {
        // A interface atual de Data exige argc e char*.
        char* instancePath = const_cast<char*>(path.c_str());
        Data data(2, path.c_str());
        data.read();

        bool success = true;

        success &= checkCondition(
            data.getDimension() == expectedDimension,
            "dimensao igual a " + std::to_string(expectedDimension)
        );

        success &= checkCondition(
            data.getInstanceName() == expectedName,
            "nome da instancia igual a " + expectedName
        );

        for (int i = 1; i <= data.getDimension(); ++i) {
            success &= checkCondition(
                std::abs(data.getDistance(i, i)) < 1e-9,
                "diagonal da cidade " + std::to_string(i) + " igual a zero"
            );
        }

        bool symmetric = true;

        for (int i = 1; i <= data.getDimension(); ++i) {
            for (int j = 1; j <= data.getDimension(); ++j) {
                if (std::abs(
                        data.getDistance(i, j) -
                        data.getDistance(j, i)
                    ) > 1e-9) {
                    symmetric = false;
                    break;
                }
            }

            if (!symmetric) {
                break;
            }
        }

        success &= checkCondition(
            symmetric,
            "matriz de distancias simetrica"
        );

        return success;
    } catch (const std::exception& error) {
        std::cerr << "[EXCECAO] " << error.what() << '\n';
        return false;
    }
}

int main() {
    bool success = true;

    success &= testInstance(
        "instances/berlin52.tsp",
        52,
        "berlin52"
    );

    success &= testInstance(
        "instances/att48.tsp",
        48,
        "att48"
    );

    success &= testInstance(
        "instances/burma14.tsp",
        14,
        "burma14"
    );

    success &= testInstance(
        "instances/bayg29.tsp",
        29,
        "bayg29"
    );

    if (!success) {
        std::cerr << "\nAlgum teste falhou.\n";
        return 1;
    }

    std::cout << "\nTodos os testes passaram.\n";
    return 0;
}