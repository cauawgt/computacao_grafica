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

    // std::cout << "O arquivo tem " << numVertices << " vértices e "
    //           << numTriangulos << " triângulos." << std::endl;

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

camstruct::Camera arquivo::lerCamera(const std::string &caminhoArquivo)
{
    std::ifstream arquivo(caminhoArquivo);
    camstruct::Camera cam;

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: Nao foi possivel abrir o ficheiro da camara.\n";
        return cam;
    }

    std::string variavel;
    char igual; // Para descartar o sinal '='

    // Lê as 6 linhas do ficheiro camera.txt
    for (int i = 0; i < 6; i++)
    {
        arquivo >> variavel >> igual;

        if (variavel == "N")
            arquivo >> cam.N.x >> cam.N.y >> cam.N.z;
        else if (variavel == "V")
            arquivo >> cam.V.x >> cam.V.y >> cam.V.z;
        else if (variavel == "C")
            arquivo >> cam.C.x >> cam.C.y >> cam.C.z;
        else if (variavel == "d")
            arquivo >> cam.d;
        else if (variavel == "hx")
            arquivo >> cam.hx;
        else if (variavel == "hy")
            arquivo >> cam.hy;
    }

    arquivo.close();
    return cam;
}