/**
 * @brief Ponto de entrada do algoritmo heurístico para o TSP.
 * 
 * O programa recebe uma instância TSPLIB, constrói uma solução inicial por
 * meio da heurística de inserção mais barata aleatorizada e imprime a rota
 * obtida com seu respectivo custo.
 *
 * Posteriormente, serão adicionados busca local, perturbação e o framework ILS.
 */ 
 
 #include "Construction.h"
 #include "Data.h"
 #include "Solution.h"

 #include <exception>
 #include <iostream>
 #include <numeric>

 namespace {

    /**
     * Uma semente fixa torna a execução reproduzível: duas execuções sobre a
     * mesma instância devem produzir a mesma solução.
     */
    constexpr unsigned int defaultSeed = 42;

    void printUsage(const char* executableName){
        std::cerr
            << "Uso: " << executableName
            << " <caminho-da-instancia.tsp>\n";
    }
 }

 /**
  * @brief Carrega uma instância e constrói uma solução inicial para o TSP.
  *
  * Fluxo da execução ->    1. valida os argumentos;
  *                         2. lê a instância TSPLIB;
  *                         3. inicializa o gerador pseudoaleatório;
  *                         4. executa a construção gulosa aleatorizada;
  *                         5. valida e imprime a solução.
  */
 int main(int argc, char* argv[]) {
    constexpr int expectedArgumentCount = 2;

    // argv[0] contém o nome do executável e argv[1] deve conter a instância.
    if (argc != expectedArgumentCount) {
        printUsage(argv[0]);
        return 1;
    }

    try {
        // Carrega a instância e prepara sua matriz de distâncias.
        Data data(argc, argv[1]);
        data.read();

        std::mt19937 randomGenerator(defaultSeed);

        const Solution solution = constructSolution(data, randomGenerator);

        /*
         * A validação protege as próximas etapas contra soluções incompletas,
         * cidades duplicadas, índices inválidos ou ciclos que não retornam
         * à cidade inicial. Desssa forma, deixando explícito que appenas uma 
         * solução válida pode ser apresentada como resultado.
         */
        if (!isValidSolution(solution, data.getDimension())) {
            std::cerr << "Erro: a solução criada é inválida.\n";
            return 1;
        }

        std::cout
            << "Instância: " << data.getInstanceName() << '\n'
            << "Dimensão: " << data.getDimension() << '\n'
            << "Semente: " << defaultSeed << '\n'
            << "Metodo: Construção gulosa aleatorizada\n";

        printSolution(solution, std::cout);

        return 0;
    } catch (const std::exception& error) {
        /* 
         * Centraliza o tratamento dos erros lançados pelo leitor, apresentando 
         * uma mensagem em vez de encerrar o programa abruptamente, como arquivo
         * inexistente, instância malformada ou formato TSLIB não suportado.
         */
        std::cerr << "Erro: " << error.what() << '\n';
        return 1;
    }
 }