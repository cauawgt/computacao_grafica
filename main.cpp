#include <iostream>
#include "include/AlgLinear.h"
#include "include/Utils.h"
#include "include/Rasteirizador.h"

int main()
{
    int comando;
    bool run = true;

    std::cout << "\n\n--- PROJETO - COMPUTACÃO GRÁFICA ---\n";

    // Carrega o modelo 3D
    estrutura::Malha malha = arquivo::lerObjeto("../data/objeto.txt");

    // Configura a sua Câmera
    camstruct::Camera cam = arquivo::lerCamera("../data/camera.txt");
    // camstruct::Camera cam;
    // cam.C = {0, -500, 500};
    // cam.N = {0, 1, -1};
    // cam.V = {0, -1, -1};
    // cam.d = 5.0f;
    // cam.hx = 2.0f;
    // cam.hy = 2.0f;

    while (run)
    {
        // Cria a imagem em memória (ex: 800x600)
        int resX = 800;
        int resY = 600;
        rasterizador::Imagem tela = rasterizador::criarImagem(resX, resY);

        // MENU
        std::cout << "\nMENU\n";
        std::cout << "0 - Executar \n";
        std::cout << "1 - Recarregar dados do Objeto\n";
        std::cout << "2 - Recarregar parâmetros da Câmera\n";
        std::cout << "3 - Parâmetros da Câmera\n";
        std::cout << "4 - Finalizar programa\n";

        std::cout << "Comando: ";
        std::cin >> comando;

        switch (comando)
        {
        case 0:

            // Chama a sua função exata do Camera.cpp para arrumar a base
            pipeline::ortogonalizarCamera(cam);

            // O Pipeline Principal
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

            // Salva a imagem gerada
            rasterizador::salvarPPM(tela, "../out/resultado.ppm");
            std::cout << "Renderizacao concluida! Abra o arquivo resultado.ppm\n\n";
            break;
        case 1:
            malha = arquivo::lerObjeto("../data/objeto.txt");
            std::cout << "Dados do objeto foram recarrecados!\n\n";
            break;
        case 2:
            cam = arquivo::lerCamera("../data/camera.txt");
            std::cout << "Parãmetros da câmera foram recarrecados!\n\n";
            break;
        case 3:
            /* code */
            std::cout << "Não implementado\n";
            break;
        case 4:
            run = false;
            break;
        default:
            std::cout << "Comando não identificado.\n\n";
            break;
        }
    }

    return 0;
}