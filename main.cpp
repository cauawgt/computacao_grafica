#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <limits>
#include "include/AlgLinear.h"
#include "include/Utils.h"
#include "include/Rasteirizador.h"
#include "include/Iluminacao.h"

// Renderização - passo a passo
rasterizador::Imagem renderizar(const estrutura::Malha &malha,
                                camstruct::Camera cam, // cópia: a ortogonalização não altera o original
                                const estrutura::Iluminacao &luz,
                                int resX, int resY)
{
    using estrutura::Ponto2D;
    using estrutura::Vetor3;

    rasterizador::Imagem tela = rasterizador::criarImagem(resX, resY);

    // ortogonalizar V, normalizar V e N, calcular U
    pipeline::ortogonalizarCamera(cam);

    //mudança mundial -> vista (UMA vez para cada vértice, e também para a luz Pl)
    std::vector<Vetor3> vistaV;
    vistaV.reserve(malha.vertices.size());
    for (const auto &v : malha.vertices)
        vistaV.push_back(pipeline::mundoParaVista(v, cam));

    estrutura::Iluminacao luzVista = luz;
    luzVista.Pl = pipeline::mundoParaVista(luz.Pl, cam);

    //normal de cada triângulo = (v2 - v1) x (v3 - v1), normalizada
    std::vector<Vetor3> normalTri(malha.triangulos.size());
    for (size_t i = 0; i < malha.triangulos.size(); ++i)
    {
        const auto &t = malha.triangulos[i];
        Vetor3 a = opalg::subtracao(vistaV[t.v2], vistaV[t.v1]);
        Vetor3 b = opalg::subtracao(vistaV[t.v3], vistaV[t.v1]);
        normalTri[i] = opesp::normalizar(opesp::produtoVetorial(a, b));
    }

    //normal de cada vértice = soma das normais dos triângulos que o contêm, normalizada
    std::vector<Vetor3> normalVert(vistaV.size(), Vetor3{0.0f, 0.0f, 0.0f});
    for (size_t i = 0; i < malha.triangulos.size(); ++i)
    {
        const auto &t = malha.triangulos[i];
        normalVert[t.v1] = opalg::adicao(normalVert[t.v1], normalTri[i]);
        normalVert[t.v2] = opalg::adicao(normalVert[t.v2], normalTri[i]);
        normalVert[t.v3] = opalg::adicao(normalVert[t.v3], normalTri[i]);
    }
    for (auto &n : normalVert)
        n = opesp::normalizar(n);

    // preparar z-buffer (uma entrada por pixel, iniciada com "infinito")
    std::vector<float> zbuffer(resX * resY, std::numeric_limits<float>::infinity());

    // ordenar os triângulos pelo baricentro (z do baricentro, mais próximo primeiro)
    std::vector<float> zBaricentro(malha.triangulos.size());
    for (size_t i = 0; i < malha.triangulos.size(); ++i)
    {
        const auto &t = malha.triangulos[i];
        zBaricentro[i] = (vistaV[t.v1].z + vistaV[t.v2].z + vistaV[t.v3].z) / 3.0f;
    }
    std::vector<int> ordem(malha.triangulos.size());
    std::iota(ordem.begin(), ordem.end(), 0);
    std::sort(ordem.begin(), ordem.end(),
              [&](int a, int b)
              { return zBaricentro[a] < zBaricentro[b]; });

    // para cada triângulo
    for (int idx : ordem)
    {
        const auto &t = malha.triangulos[idx];

        // Vértices e normais do triângulo, em coordenadas de vista
        Vetor3 P1 = vistaV[t.v1], P2 = vistaV[t.v2], P3 = vistaV[t.v3];
        Vetor3 N1 = normalVert[t.v1], N2 = normalVert[t.v2], N3 = normalVert[t.v3];

        // Proteção: sem recorte, vértices atrás da câmera (z <= 0)
        // projetariam pontos espelhados/inválidos. Triângulos assim são ignorados.
        if (P1.z <= 0.0f || P2.z <= 0.0f || P3.z <= 0.0f)
            continue;

        // projeta os vértices (perspectiva -> NDC -> tela)
        auto projetar = [&](const Vetor3 &pv) -> Ponto2D
        {
            Vetor3 proj = pipeline::projetarEmPerspectiva(pv, cam.d);
            Vetor3 ndc = pipeline::coordenadasNDC(proj, cam.hx, cam.hy);
            return pipeline::coordenadasTela(ndc, resX, resY);
        };
        Ponto2D p1 = projetar(P1);
        Ponto2D p2 = projetar(P2);
        Ponto2D p3 = projetar(P3);

        // dentro do scanline, para cada pixel (x, y) da lista de pixels de um segmento
        auto pintar = [&](int x, int y)
        {
            if (x < 0 || x >= resX || y < 0 || y >= resY)
                return;

            // Coordenadas baricêntricas com respeito aos vértices projetados
            float alfa, beta, gama;
            if (!rasterizador::baricentricas({x, y}, p1, p2, p3, alfa, beta, gama))
                return;
            if (alfa < 0.0f || beta < 0.0f || gama < 0.0f) // ponto fora do triângulo
                return;

            //encontra P original  ~  alfa*P1 + beta*P2 + gama*P3
            Vetor3 P = opalg::adicao(
                opalg::adicao(opalg::multEscalar(alfa, P1), opalg::multEscalar(beta, P2)),
                opalg::multEscalar(gama, P3));

            // Consulta/atualiza o z-buffer: só continua se z < atual
            int pos = y * resX + x;
            if (P.z >= zbuffer[pos])
                return;
            zbuffer[pos] = P.z;

            //encontra N = alfa*N1 + beta*N2 + gama*N3, e normaliza
            Vetor3 N = opesp::normalizar(opalg::adicao(
                opalg::adicao(opalg::multEscalar(alfa, N1), opalg::multEscalar(beta, N2)),
                opalg::multEscalar(gama, N3)));

            // V, L, R, casos especiais e cor (modelo de iluminação de Phong)
            Vetor3 cor = iluminacao::phong(P, N, luzVista);

            rasterizador::pintarPixel(tela, x, y, cor.x, cor.y, cor.z);
        };

        rasterizador::preencherTriangulo(p1, p2, p3, pintar);
    }

    return tela;
}

int main()
{
    int comando;
    bool run = true;

    std::cout << "\n\n--- PROJETO - COMPUTACÃO GRÁFICA ---\n";

    // Caminhos calculados na hora, a partir da raiz do projeto (a pasta que contém data/).
    // Assim funciona rodando da raiz, de build/ ou de build/Debug/.
    auto caminhoObjeto = [] { return arquivo::raizProjeto() / "data" / "objeto.txt"; };
    auto caminhoCamera = [] { return arquivo::raizProjeto() / "data" / "camera.txt"; };
    auto caminhoSaida  = [] { return arquivo::raizProjeto() / "out" / "resultado.ppm"; };

    // carrega vértices e triângulos
    estrutura::Malha malha;
    arquivo::lerObjeto(caminhoObjeto(), malha);

    // carrega parâmetros da câmera e da iluminação
    camstruct::Camera cam{};
    estrutura::Iluminacao luz{};
    arquivo::lerParametros(caminhoCamera(), cam, luz);
    // camstruct::Camera cam;
    // cam.C = {0, -500, 500};
    // cam.N = {0, 1, -1};
    // cam.V = {0, -1, -1};
    // cam.d = 5.0f;
    // cam.hx = 2.0f;
    // cam.hy = 2.0f;

    const int resX = 800;
    const int resY = 600;

    while (run)
    {
        // MENU
        std::cout << "\nMENU\n";
        std::cout << "0 - Executar \n";
        std::cout << "1 - Recarregar dados do Objeto\n";
        std::cout << "2 - Recarregar parâmetros da Câmera e da Iluminação\n";
        std::cout << "3 - Parâmetros da Câmera e da Iluminação\n";
        std::cout << "4 - Finalizar programa\n";

        std::cout << "Comando: ";
        std::cin >> comando;

        switch (comando)
        {
        case 0:
        {
            rasterizador::Imagem tela = renderizar(malha, cam, luz, resX, resY);
            auto saida = caminhoSaida();
            if (rasterizador::salvarPPM(tela, saida))
                std::cout << "Renderizacao concluida! Abra o arquivo " << saida.string() << "\n\n";
            else
                std::cout << "Erro: nao foi possivel salvar " << saida.string() << "\n\n";
            break;
        }
        case 1:
            if (arquivo::lerObjeto(caminhoObjeto(), malha))
                std::cout << "Dados do objeto foram recarregados!\n\n";
            else
                std::cout << "Falha ao recarregar o objeto (dados anteriores mantidos).\n\n";
            break;
        case 2:
            if (arquivo::lerParametros(caminhoCamera(), cam, luz))
                std::cout << "Parâmetros da câmera e da iluminação foram recarregados!\n\n";
            else
                std::cout << "Falha ao recarregar os parâmetros (valores anteriores mantidos).\n\n";
            break;
        case 3:
            std::cout << "C    = " << visualizacao::vetor(cam.C);
            std::cout << "N    = " << visualizacao::vetor(cam.N);
            std::cout << "V    = " << visualizacao::vetor(cam.V);
            std::cout << "d = " << cam.d << " | hx = " << cam.hx << " | hy = " << cam.hy << "\n";
            std::cout << "Iamb = " << visualizacao::vetor(luz.Iamb);
            std::cout << "Il   = " << visualizacao::vetor(luz.Il);
            std::cout << "Pl   = " << visualizacao::vetor(luz.Pl);
            std::cout << "Kd   = " << visualizacao::vetor(luz.Kd);
            std::cout << "Od   = " << visualizacao::vetor(luz.Od);
            std::cout << "Ka = " << luz.Ka << " | Ks = " << luz.Ks << " | eta = " << luz.eta << "\n";
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
