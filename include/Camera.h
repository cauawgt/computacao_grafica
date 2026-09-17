#pragma once
#include "AlgLinear.h"

namespace camstruct
{
    struct Camera
    {
        estrutura::Vetor3 C; // Posição (Foco)
        estrutura::Vetor3 N; // Vetor Normal (Para onde Olha)
        estrutura::Vetor3 V; // Vetor View-Up (cima)
        estrutura::Vetor3 U; // Vetor Ortogonal (direita)
        float d, hx, hy;     // distância focal e dimensões da tela
    };

}

namespace pipeline
{
    // Função para corrigir os vetores lidos do arquivo (Gram-Schmidt)
    void ortogonalizarCamera(camstruct::Camera &cam);

    // Função que Converte um ponto 3D do mundo para a visão da câmera
    estrutura::Vetor3 mundoParaVista(estrutura::Vetor3 P, camstruct::Camera cam);

    // Projeção em Perspectiva
    estrutura::Vetor3 projetarEmPerspectiva(estrutura::Vetor3 pVista, float d);

    // Coordenadas Normalizadas - NDC
    estrutura::Vetor3 coordenadasNDC(estrutura::Vetor3 pProjetado, float hx, float hy);

    // Coordenadas de Tela
    estrutura::Ponto2D coordenadasTela(estrutura::Vetor3 pNDC, int resX, int resY);

}
