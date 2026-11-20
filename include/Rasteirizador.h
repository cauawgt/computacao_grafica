#pragma once
#include "Camera.h"
#include <vector>
#include <string>
#include <functional>
#include <filesystem>

namespace rasterizador {
    // Uma matriz simples que representa a nossa tela
    struct Imagem {
        int largura, altura;
        std::vector<estrutura::Vetor3> pixels; // Guarda as cores RGB (0 a 255)
    };

    // Inicializa a tela com a cor preta
    Imagem criarImagem(int largura, int altura);

    // Salva a tela no formato PPM para você poder ver a imagem depois!
    // Retorna true se conseguiu gravar o arquivo.
    bool salvarPPM(const Imagem& img, const std::filesystem::path& nomeArquivo);

    // Nossa única função de acender pixel permitida
    void pintarPixel(Imagem& img, int x, int y, float r, float g, float b);

    // Varre (scanline) o triângulo e, para cada pixel (x, y) dentro dele, chama 'pintar'.
    // Quem decide a cor (baricêntricas, z-buffer, Phong) é essa função.
    void preencherTriangulo(estrutura::Ponto2D p1, estrutura::Ponto2D p2, estrutura::Ponto2D p3,
                            const std::function<void(int, int)>& pintar);

    // Coordenadas baricêntricas (alfa, beta, gama) de P em relação
    // ao triângulo A, B, C (Pontos projetados na tela)
    // Retorna false se o triângulo for degenerado (área zero)
    bool baricentricas(estrutura::Ponto2D P,
                        estrutura::Ponto2D A, estrutura::Ponto2D B, estrutura::Ponto2D C,
                        float  &alfa, float &beta, float &gama);
}