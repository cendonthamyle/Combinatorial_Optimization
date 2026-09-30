# Combinatorial Optimization

Implementações desenvolvidas para resolução do kit de Otimização Combinatória da UFPB.

## Estrutura

- `common`: componentes reutilizados pelos diferentes capítulos, começando pelo
leitor de instâncias TSPLIB.
- `01_tsp`: heurística ILS-RVND aplicada ao TSP.
- `02-mlp`: heurística para o Problema da Latência Mínima.
- `03-branch-and-bound`: algoritmo Branch-and-Bound combinatório.
- `04-lagrangian-relaxation`: relaxação lagrangiana.
- `05-branch-and-cut`: algoritmo Branch-and-Cut.
- `instances`: instâncias da TSPLIB usadas como dados de entrada.

## Requisitos

- Compilador com suporte a C++17
- CMake 3.16 ou superior

## Compilação

```sh
cmake -S . -B build
cmake --build build
```