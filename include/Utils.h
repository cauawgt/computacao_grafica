#pragma once
#include <iostream>
#include "AlgLinear.h"
#include "Camera.h"

namespace visualizacao {
    std::string vetor(estrutura::Vetor3 v);
}

namespace arquivo
{
    estrutura::Malha lerObjeto(const std::string& caminhoArquivo);
    camstruct::Camera lerCamera(const std::string& caminhoArquivo);
} 
