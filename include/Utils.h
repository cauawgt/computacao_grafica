#pragma once
#include <iostream>
#include "AlgLinear.h"

namespace visualizacao {
    std::string vetor(estrutura::Vetor3 v);
}

namespace arquivo
{
    estrutura::Malha lerObjeto(const std::string& caminhoArquivo);
} 
