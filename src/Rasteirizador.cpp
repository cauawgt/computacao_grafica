#include "../include/Rasteirizador.h"
#include <fstream>
#include <algorithm>
#include <math.h>
#include <utility> // Para o std::swap

rasterizador::Imagem rasterizador::criarImagem(int largura, int altura)
{
    Imagem img;
    img.largura = largura;
    img.altura = altura;
    // Cria um array de pixels (todos pretos por padrão)
    img.pixels.resize(largura * altura, {0.0f, 0.0f, 0.0f});
    return img;
}

void rasterizador::salvarPPM(const Imagem &img, const std::string &nomeArquivo)
{
    std::ofstream arquivo(nomeArquivo);
    arquivo << "P3\n"
            << img.largura << " " << img.altura << "\n255\n";
    for (const auto &cor : img.pixels)
    {
        arquivo << (int)cor.x << " " << (int)cor.y << " " << (int)cor.z << "\n";
    }
    arquivo.close();
}

void rasterizador::pintarPixel(Imagem &img, int x, int y, float r, float g, float b)
{
    if (x >= 0 && x < img.largura && y >= 0 && y < img.altura)
    {
        int indice = y * img.largura + x;
        img.pixels[indice] = {r, g, b};
    }
}

// SCAN LINE (VARREDURA)
void rasterizador::preencherTriangulo(Imagem &img, estrutura::Ponto2D p1, estrutura::Ponto2D p2, estrutura::Ponto2D p3)
{

    // Ordenar os vértices pela coordenada Y (de cima para baixo na tela)
    // Isso garante que p1 é o ponto mais alto, p2 o do meio e p3 o mais baixo
    if (p1.y > p2.y)
        std::swap(p1, p2);
    if (p1.y > p3.y)
        std::swap(p1, p3);
    if (p2.y > p3.y)
        std::swap(p2, p3);

    // Prevenção: Se o triângulo for uma linha horizontal perfeita (altura 0), ignoramos
    if (p1.y == p3.y)
        return;
    
    // Calcular o INVERSO do declive (1/a) para cada aresta do triângulo
    // 1/a = (x1 - x0) / (y1 - y0). Usado para atualizar o X a cada descida no Y.
    float invA1, invA2, InvA3;
    invA1 = (p2.y - p1.y > 0) ? ((float)(p2.x - p1.x)) / ((float)(p2.y - p1.y)) : 0.0f;
    invA2 = (p3.y - p1.y > 0) ? ((float)(p3.x - p1.x)) / ((float)(p3.y - p1.y)) : 0.0f;
    InvA3 = (p3.y - p2.y > 0) ? ((float)(p3.x - p2.x)) / ((float)(p3.y - p2.y)) : 0.0f;

    float atualX1 = p1.x; // Rastreia o X da esquerda (ou direita)
    float atualX2 = p1.x; // Rastreia o X da direita (ou esquerda)

    // Rasterizar o primeiro triângulo menor (parte superior: do P1 ao P2)
    for (int y = p1.y; y < p2.y; ++y)
    { // Usa '<' para não pintar a junção duas vezes
        int x_min = std::min(atualX1, atualX2);
        int x_max = std::max(atualX1, atualX2);

        // Scanline: Sequência de retas de pixels horizontais
        for (int x = x_min; x <= x_max; ++x)
        {
            pintarPixel(img, x, y, 255, 255, 255);
        }

        // Equação de atualização: x = 1/a + x
        atualX1 += invA1;
        atualX2 += invA2;
    }

    // Rasterizar o SEGUNDO TRIÂNGULO menor (parte inferior: do P2 ao P3)
    // A aresta 1 (p1->p2) acabou no p2.y. Agora o curX1 precisa acompanhar a aresta 3 (p2->p3).
    atualX1 = p2.x;

    for (int y = p2.y; y <= p3.y; ++y)
    {
        int x_min = std::min(atualX1, atualX2);
        int x_max = std::max(atualX1, atualX2);

        // Scanline
        for (int x = x_min; x <= x_max; ++x)
        {
            pintarPixel(img, x, y, 255, 255, 255);
        }

        // Equação de atualização: x = 1/a + x
        atualX1 += InvA3;
        atualX2 += invA2;
    }
}