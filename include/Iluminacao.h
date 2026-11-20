#pragma once
#include "AlgLinear.h"

namespace iluminacao {
    // P: ponto original em coordenadas de VISTA 
    // N: normal em P, já normalizada
    // luz: parâmetros de iluminação, com luz.Pl JÁ em coordenadas de vista
    // Retorna a cor RGB (0..255), já limitada a 255.
    estrutura::Vetor3 phong(estrutura::Vetor3 P,
                            estrutura::Vetor3 N,
                            const estrutura::Iluminacao &luz);
}