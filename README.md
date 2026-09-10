# Combinatorial Optimization

Implementações desenvolvidas a partir do kit de Otimização Combinatória, usando
instâncias da TSPLIB.

## Estado atual

- [x] Leitor de instâncias TSPLIB
- [ ] Heurística ILS-RVND para o TSP
- [ ] Problema da Latência Mínima (MLP)
- [ ] Branch-and-Bound combinatório
- [ ] Relaxação Lagrangiana
- [ ] Branch-and-Cut

## Estrutura

- `common`: componentes reutilizados pelos diferentes capítulos, começando pelo
leitor de instâncias TSPLIB.
- `01_tsp`: heurística ILS-RVND aplicada ao TSP.
- `02-mlp`: heurística para o Problema da Latência Mínima.
- `03-branch-and-bound`: algoritmo Branch-and-Bound combinatório.
- `04-lagrangian-relaxation`: relaxação lagrangiana.
- `05-branch-and-cut`: algoritmo Branch-and-Cut.
- `instances`: instâncias da TSPLIB usadas como dados de entrada.
- `scripts`: automação dos experimentos e consolidação dos resultados.
- `docs`: notas de implementação e metodologia experimental.

Cada capítulo possui as pastas `include`, `src`, `tests` e `results`. Somente
componentes efetivamente compartilhados devem ser colocados em `common`.

## Requisitos

- Compilador com suporte a C++17
- CMake 3.16 ou superior

## Compilação

```sh
cmake -S . -B build
cmake --build build
```

Neste estágio, a compilação gera apenas a biblioteca `tsplib_reader`, a partir de
`common/src/Data.cpp`. O executável do TSP será adicionado em
`01-tsp-gils-rvnd` na próxima etapa.
