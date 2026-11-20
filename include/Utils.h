#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include "AlgLinear.h"
#include "Camera.h"

namespace visualizacao {
    std::string vetor(estrutura::Vetor3 v);
}

namespace arquivo
{
    // Procura a pasta raiz do projeto (a que contém data/) a partir da pasta atual,
    // subindo alguns níveis. Assim o programa funciona rodando da raiz, de build/
    // ou de build/Debug/. Se não achar, devolve a pasta atual.
    std::filesystem::path raizProjeto();

    // Retorna true se leu o arquivo. Em caso de erro, "malha" não é alterada.
    bool lerObjeto(const std::filesystem::path& caminhoArquivo, estrutura::Malha& malha);
    // Lê câmera (C, N, V, d, hx, hy) E iluminação (Iamb, Ka, Il, Pl, Kd, Od, Ks, eta)
    // do mesmo arquivo de texto. Retorna true se conseguiu abrir o arquivo.
    bool lerParametros(const std::filesystem::path& caminhoArquivo,
                       camstruct::Camera& cam,
                       estrutura::Iluminacao& luz);
} 
