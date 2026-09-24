/* Este programa:
 * 1. recebe o caminho de uma instância TSPLIB;
 * 2. carrega sua matriz de distâncias;
 * 3. cria uma solução sequencial provisória;
 * 4. calcula e valida seu custo;
 * 5. imprime o resultado.
 *
 * A solução sequencial será substituída posteriormente pela heurística
 * construtiva gulosa aleatorizada apresentada na Seção 2.2 do kit.
 */

 #include "Data.h"
 #include "Solution.h"

 #include <exception>
 #include <iostream>
 #include <numeric>

 namespace {
    void printUsage(const char* executableName){
        std::cerr
            << "Uso: " << executableName
            << " <caminho-da-instancia.tsp>\n";
    }
 }

 int main(int argc, char* argv[]) {
    constexpr int expectedArgumentCount = 2;

    // argv[0] contém o nome do executável e argv[1] deve conter a instância.
    if (argc != expectedArgumentCount) {
        printUsage(argv[0]);
        return 1;
    }

    try {
        // Carrega a instância e constrói sua matriz de distâncias.
        Data data(argc, argv[1]);
        data.read();

        Solution solution;

        /*
         * Cria a solução provisória [1, 2, 3, ..., n].
         *
         * std::iota atribui valores consecutivos ao vetor, começãndo em 1.
         * Por exemplo, para quatro cidade, o resultado é [1, 2, 3, 4].
         */
        solution.sequence.resize(data.getDimension());

        std::iota(
            solution.sequence.begin(),
            solution.sequence.end(),
            1
        );

        /*
         * Repete a primeira cidade no final para representar explicitamente
         * o retorno ao ponto ao partido: [1, 2, ..., n, 1].
         */
        solution.sequence.push_back(1);

        // Soma as distâncias entre todos os pares de cidades consecutivas.
        solution.cost = calculateCost(solution, data);

        /*
         * A validação protege as próximas etapas contra soluções incompletas,
         * cidades duplicadas, índices inválidos ou ciclos que não retornam
         * à cidade inicial.
         */
        if (!isValidSolution(solution, data.getDimension())) {
            std::cerr << "Erro: a solução criada é inválida.\n";
            return 1;
        }

        std::cout
            << "Instância: " << data.getInstanceName() << '\n'
            << "Dimensão: " << data.getDimension() << '\n';

        printSolution(solution, std::cout);

        return 0;
    } catch (const std::exception& error) {
        /* 
         * Centraliza o tratamento dos erros lançados pelo leitor, como arquivo
         * inexistente, instância malformada ou formato TSLIB não suportado.
         */
        std::cerr << "Erro: " << error.what() << '\n';
        return 1;
    }
 }