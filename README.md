# Implementação do Team Orienteering Problem (TOP)

Este repositório contém uma implementação do Team Orienteering Problem (TOP) utilizando o CPLEX.

## Descrição

O Team Orienteering Problem (TOP) é um problema de otimização combinatória onde o objetivo é maximizar a soma dos prêmios coletados por um conjunto de rotas, respeitando restrições de tempo e capacidade.

### O Problema

No TOP, temos um conjunto de locais, cada um associado a um prêmio. Um número fixo de veículos (ou equipes) parte de um ponto inicial e deve retornar a um ponto final dentro de um limite de tempo. Cada veículo pode visitar um subconjunto dos locais, e o objetivo é determinar as rotas de cada veículo de forma que a soma dos prêmios coletados seja maximizada, sem exceder o limite de tempo para cada rota.

### Aplicações

O TOP tem várias aplicações práticas, incluindo:
- Planejamento de rotas para veículos de entrega.
- Planejamento de rotas turísticas.
- Planejamento de rotas para serviços de manutenção.

## Estrutura do Projeto

- `main.cpp`: Arquivo principal contendo a implementação do algoritmo TOP.
- `Makefile`: Arquivo para compilação do projeto.
- `instances/`: Diretório contendo as instâncias do problema em arquivos `.txt`.
- `test.sh`: Script para execução de testes.
- `README.md`: Este arquivo.

## Compilação e Execução

Para compilar o projeto, utilize o comando:
```sh
make
```