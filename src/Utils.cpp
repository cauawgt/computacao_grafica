#include <sstream>
#include "../include/Utils.h"
#include "../include/AlgLinear.h"
#include <iostream>
#include <fstream>
#include <string>

std::string visualizacao::vetor(estrutura::Vetor3 v)
{
    std::stringstream ss;
    ss << "(" << v.x << ", " << v.y << ", " << v.z << ")" << "\n";
    return ss.str();
}

estrutura::Malha arquivo::lerObjeto(const std::string &caminhoArquivo)
{
    std::ifstream arquivo(caminhoArquivo);
    estrutura::Malha malha;

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << caminhoArquivo << std::endl;
        return malha; // Retorna malha vazia se der erro
    }

    int numVertices, numTriangulos;

    // Lê a primeira linha: Quantidades
    arquivo >> numVertices >> numTriangulos;

    std::cout << "O arquivo tem " << numVertices << " vértices e "
              << numTriangulos << " triângulos." << std::endl;

    // Laço para ler todos os vértices (X, Y, Z)
    for (int i = 0; i < numVertices; i++)
    {
        estrutura::Vetor3 v;
        arquivo >> v.x >> v.y >> v.z;
        malha.vertices.push_back(v);
    }

    // Laço para ler todos os triângulos (Índices)
    for (int i = 0; i < numTriangulos; i++)
    {
        estrutura::Triangulo t;
        arquivo >> t.v1 >> t.v2 >> t.v3;

        // a contagem comeca no índice 1.
        // Mas os arrays (std::vector) em C++ começam no zero
        // Portanto, nós subtraímos 1 para alinhar a matemática com a programação:
        t.v1 -= 1;
        t.v2 -= 1;
        t.v3 -= 1;

        malha.triangulos.push_back(t);
    }

    arquivo.close();
    std::cout << "Malha carregada com sucesso! Vértices: " << numVertices
              << " | Triângulos: " << numTriangulos << std::endl;

    return malha;
}