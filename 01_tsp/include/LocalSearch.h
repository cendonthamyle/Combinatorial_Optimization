#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "Solution.h"
#include <cstddef>
#include <random>

class Data;

/**
 * @brief Aplica o melhor movimento Swap encontrado na solução.
 *
 * O movimento Swap troca as posições de duas cidades internas do tour.
 * A cidade inicial, repetida no início e no final da sequência, permanece fixa.
 *
 * Todas as trocas possíveis são avaliadas, mas somente aquela que produz
 * a maior redução no custo é aplicada.
 *
 * A avaliação utiliza apenas as arestas afetadas pela troca, evitando
 * recalcular o custo completo de cada solução candidata.
 */
bool bestImprovementSwap(
    Solution& solution,
    const Data& data
);

/**
 * @brief Aplica o melhor movimento 2-opt encontrado na solução.
 *
 * O movimento remove duas arestas e reconecta o tour invertendo o segmento
 * localizado entre elas. Todas as combinações válidas são avaliadas, mas
 * somente a que produz a maior redução no custo é aplicada.
 *
 * A avaliação do delta considera apenas as duas arestas removidas e as duas
 * arestas adicionadas. Essa simplificação é válida para o TSP simétrico,
 * pois inverter a direção das arestas internas não altera seus custos.
 */
bool bestImprovement2Opt(
    Solution& solution,
    const Data& data
);

/**
 * @brief Aplica o melhor movimento de remoção e reinserção de um bloco.
 *
 * Um bloco de cidades consecutivas é retirado de sua posição atual e
 * reinserido em outra aresta da rota, preservando sua ordem interna.
 *
 * O tamanho do bloco determina 
 * a estrutura de vizinhança    -> 1: Reinsertion;
 *                                 2: Or-opt-2;
 *                                 3: Or-opt-3.
 *
 * Todas as combinações válidas de bloco e posição de inserção são avaliadas,
 * mas somente o movimento que produz a maior redução de custo é aplicado.
 
 * @throws std::invalid_argument Se blockSize não estiver entre 1 e 3.
 */
bool bestImprovementOrOpt(
    Solution& solution,
    const Data& data,
    std::size_t blockSize
);

/**
 * @brief Melhora uma solução usando Random Variable Neighborhood Descent.
 *
 * O RVND mantém uma lista 
 * com as cinco estruturas 
 * de vizinhança: Swap;
 *                2-opt;
 *                Reinsertion;
 *                Or-opt-2;
 *                Or-opt-3.
 * 
 * Em cada iteração, uma vizinhança é escolhida aleatoriamente. Se ela
 * melhorar a solução, a lista completa é restaurada, pois a modificação
 * pode ter criado novas oportunidades nas vizinhanças já exploradas.
 *
 * Se não houver melhoria, a vizinhança é removida da lista corrente.
 * O procedimento termina quando todas as vizinhanças falham em melhorar
 * a mesma solução.
 */
void randomVariableNeighborhoodDescent(
    Solution& solution,
    const Data& data,
    std::mt19937& randomGenerator
);
#endif