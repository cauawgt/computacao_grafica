#include <iostream>
#include "include/AlgLinear.h"
#include "include/Utils.h"
#include "include/Rasteirizador.h"

int main()
{
    // 1. Carrega o modelo 3D (Certifique-se de ter um arquivo 'objeto.txt' na pasta!)
    // A sua função lerObjeto já está lá no ArquivoUtilidades.cpp
    estrutura::Malha malha = arquivo::lerObjeto("../data/objeto.txt");

    // 2. Cria a imagem em memória (ex: 800x600)
    int resX = 800;
    int resY = 600;
    rasterizador::Imagem tela = rasterizador::criarImagem(resX, resY);

    // 3. Configura a sua Câmera (Com os valores fictícios do Slide 12)
    camstruct::Camera cam;
    cam.C = {1, 1, 2};
    cam.N = {-1, -1, -1};
    cam.V = {0, 0, 1};
    cam.d = 1.0f;
    cam.hx = 1.0f;
    cam.hy = 1.0f;

    // Chama a sua função exata do Camera.cpp para arrumar a base
    pipeline::ortogonalizarCamera(cam);

    // 4. O Pipeline Principal
    for (const auto &triangulo : malha.triangulos)
    {

        // Pega os 3 vértices 3D
        estrutura::Vetor3 v1 = malha.vertices[triangulo.v1];
        estrutura::Vetor3 v2 = malha.vertices[triangulo.v2];
        estrutura::Vetor3 v3 = malha.vertices[triangulo.v3];

        // Passa o Vértice 1 por todas as suas funções da Camera.cpp
        estrutura::Vetor3 vista1 = pipeline::mundoParaVista(v1, cam);
        estrutura::Vetor3 proj1 = pipeline::projetarEmPerspectiva(vista1, cam.d);
        estrutura::Vetor3 ndc1 = pipeline::coordenadasNDC(proj1, cam.hx, cam.hy);
        estrutura::Ponto2D p1 = pipeline::coordenadasTela(ndc1, resX, resY);

        // Passa o Vértice 2
        estrutura::Vetor3 vista2 = pipeline::mundoParaVista(v2, cam);
        estrutura::Vetor3 proj2 = pipeline::projetarEmPerspectiva(vista2, cam.d);
        estrutura::Vetor3 ndc2 = pipeline::coordenadasNDC(proj2, cam.hx, cam.hy);
        estrutura::Ponto2D p2 = pipeline::coordenadasTela(ndc2, resX, resY);

        // Passa o Vértice 3
        estrutura::Vetor3 vista3 = pipeline::mundoParaVista(v3, cam);
        estrutura::Vetor3 proj3 = pipeline::projetarEmPerspectiva(vista3, cam.d);
        estrutura::Vetor3 ndc3 = pipeline::coordenadasNDC(proj3, cam.hx, cam.hy);
        estrutura::Ponto2D p3 = pipeline::coordenadasTela(ndc3, resX, resY);

        // Manda o Rasterizador preencher esse triângulo na Imagem!
        rasterizador::preencherTriangulo(tela, p1, p2, p3);
    }

    // 5. Salva a imagem gerada
    rasterizador::salvarPPM(tela, "/home/caua/Documents/computacao_grafica/out/resultado.ppm");
    std::cout << "Renderizacao concluida! Abra o arquivo resultado.ppm\n";

    return 0;
}