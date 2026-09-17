#include "../include/Camera.h"
#include "../include/AlgLinear.h"
#include <math.h>

void pipeline::ortogonalizarCamera(camstruct::Camera &cam)
{
    // Ortogonalizar V usando GRAM-SCHIMDT
    // V = V' (<V,N>/<N,N>) * N
    float prodEscalarVN = opesp::produtoEscalar(cam.V, cam.N);
    float prodEscalarNN = opesp::produtoEscalar(cam.V, cam.N);
    float k = prodEscalarVN - prodEscalarNN;

    estrutura::Vetor3 projecaoN = opalg::multEscalar(k, cam.N);
    estrutura::Vetor3 vLinha = opalg::subtracao(cam.V, projecaoN);

    // Calcular o vetor U através do produto vetorial: U = N x V'
    cam.U = opesp::produtoVetorial(cam.N, vLinha);

    // Normalizar os vetores U, V' e N para terem tamanho 1
    float normaU = opesp::norma(cam.U);
    cam.U = opalg::multEscalar(1.0f / normaU, cam.U);

    float normaV = opesp::norma(vLinha);
    cam.V = opalg::multEscalar(1.0f / normaV, vLinha);

    float normaN = opesp::norma(cam.N);
    cam.N = opalg::multEscalar(1.0f / normaN, cam.N);
}

estrutura::Vetor3 pipeline::mundoParaVista(estrutura::Vetor3 P, camstruct::Camera cam)
{
    // Translação: Encontrar o vetor direcional v = P - C
    estrutura::Vetor3 v = opalg::subtracao(P, cam.C);

    // Matriz de Mudança de Base [I] transposta
    // ortonormais U, V e N nas linhas da matriz
    estrutura::Matriz3x3 mat;

    // Linha 1 (Eixo X da câmera)
    mat.m[0][0] = cam.U.x;
    mat.m[0][1] = cam.U.y;
    mat.m[0][2] = cam.U.z;

    // Linha 2 (Eixo Y da câmera)
    mat.m[1][0] = cam.V.x;
    mat.m[1][1] = cam.V.y;
    mat.m[1][2] = cam.V.z;

    // Linha 3 (Eixo Z da câmera)
    mat.m[2][0] = cam.N.x;
    mat.m[2][1] = cam.N.y;
    mat.m[2][2] = cam.N.z;

    // Por fim, realizamos a multiplicação da matriz pelo vetor direcional (P - C)
    return opalg::multMatrizVetor(mat, v);
}

estrutura::Vetor3 pipeline::projetarEmPerspectiva(estrutura::Vetor3 pVista, float d) {
    estrutura::Vetor3 pProjetado;

    // Prevenção contra divisão por zero
    float z = (pVista.z == 0.0f) ? 0.0001f : pVista.z;

    // Xs = d * (Xv / Zv)  e  Ys = d * (Yv / Zv)
    pProjetado.x = d * (pVista.x / z);
    pProjetado.y = d * (pVista.y / z);
    pProjetado.z = d; // O ponto agora vive no plano de projeção

    return pProjetado;
}

estrutura::Vetor3 pipeline::coordenadasNDC(estrutura::Vetor3 pProjetado, float hx, float hy) {
    estrutura::Vetor3 ndc;

    // x_ndc = xs / hx  e  y_ndc = ys / hy
    ndc.x = pProjetado.x / hx;
    ndc.y = pProjetado.y / hy;
    ndc.z = pProjetado.z;

    return ndc;
}

estrutura::Ponto2D pipeline::coordenadasTela(estrutura::Vetor3 pNDC, int resX, int resY) {
    estrutura::Ponto2D tela;

    // Slide 25: i = floor( ((x_ndc + 1) / 2) * ResX + 0.5 )
    tela.x = static_cast<int>(std::floor(((pNDC.x + 1.0f) / 2.0f) * resX + 0.5f));

    // Slide 25: j = floor( ResY - ((y_ndc + 1) / 2) * ResY + 0.5 )
    tela.y = static_cast<int>(std::floor(resY - (((pNDC.y + 1.0f) / 2.0f) * resY) + 0.5f));

    return tela;
}