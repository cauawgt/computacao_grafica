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

std::filesystem::path arquivo::raizProjeto()
{
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path atual = fs::current_path(ec);
    if (ec)
        return fs::path(".");

    fs::path p = atual;
    for (int nivel = 0; nivel < 4; ++nivel)
    {
        if (fs::is_directory(p / "data", ec))
            return p;
        if (!p.has_parent_path() || p.parent_path() == p)
            break;
        p = p.parent_path();
    }
    return atual;
}

bool arquivo::lerObjeto(const std::filesystem::path &caminhoArquivo, estrutura::Malha &malhaSaida)
{
    std::ifstream arquivo(caminhoArquivo);
    estrutura::Malha malha;

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << caminhoArquivo.string() << std::endl;
        return false; // mantém a malha anterior
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
        // subtraímos 1 para alinhar a matemática com a programação:
        t.v1 -= 1;
        t.v2 -= 1;
        t.v3 -= 1;

        malha.triangulos.push_back(t);
    }

    if (arquivo.fail())
    {
        std::cerr << "Erro: formato invalido em " << caminhoArquivo.string() << std::endl;
        return false; // mantém a malha anterior
    }

    arquivo.close();
    std::cout << "Malha carregada com sucesso! Vértices: " << numVertices
              << " | Triângulos: " << numTriangulos << std::endl;

    malhaSaida = std::move(malha);
    return true;
}

// carregar parâmetros de câmera E iluminação.
// Formato de cada linha:  nome = valor(es)
bool arquivo::lerParametros(const std::filesystem::path &caminhoArquivo,
                            camstruct::Camera &cam,
                            estrutura::Iluminacao &luz)
{
    std::ifstream arquivo(caminhoArquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo de parametros " << caminhoArquivo.string() << "\n";
        return false;
    }

    std::string variavel;
    char igual; // Para descartar o sinal '='

    // Lê até o fim do arquivo (qualquer quantidade de linhas, em qualquer ordem)
    while (arquivo >> variavel >> igual)
    {
        // --- Câmera ---
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
        // --- Iluminação ---
        else if (variavel == "Iamb")
            arquivo >> luz.Iamb.x >> luz.Iamb.y >> luz.Iamb.z;
        else if (variavel == "Ka")
            arquivo >> luz.Ka;
        else if (variavel == "Il")
            arquivo >> luz.Il.x >> luz.Il.y >> luz.Il.z;
        else if (variavel == "Pl")
            arquivo >> luz.Pl.x >> luz.Pl.y >> luz.Pl.z;
        else if (variavel == "Kd")
            arquivo >> luz.Kd.x >> luz.Kd.y >> luz.Kd.z;
        else if (variavel == "Od")
            arquivo >> luz.Od.x >> luz.Od.y >> luz.Od.z;
        else if (variavel == "Ks")
            arquivo >> luz.Ks;
        else if (variavel == "eta") // o símbolo η do enunciado vira "eta" no arquivo
            arquivo >> luz.eta;
        else
        {
            // Variável desconhecida: ignora o resto da linha
            std::string resto;
            std::getline(arquivo, resto);
            std::cerr << "Aviso: parametro desconhecido '" << variavel << "' ignorado.\n";
        }
    }

    arquivo.close();
    return true;
}
