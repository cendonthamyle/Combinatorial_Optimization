#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include "Solution.h"
#include <random>

class Data;

/**
 * Constrói uma solução completa para o TSP por inserção ais barata.
 * 
 * O procedimente começa com um subtour formado pela cidade 1 e três cidades
 * aleatórias. As cidades restantes são inseridas uma por vez. Em cada iteração,
 * todas as inserções possíveis são avaliadas e ordenadas pelo aumento de custo.
 * 
 * Para evitar que todas as construções produzam soluções muito semelhantes,
 * a inserção aplicada é escolhida aleatoriamente dentro de uma lista restrita
 * que contém as alternativas mais promissoras
 */

 Solution constructSolution(
    const Data& data,
    std::mt19937& randomGenerator
 );

#endif