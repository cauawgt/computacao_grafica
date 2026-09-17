#pragma once

namespace estrutura
{
    struct Ponto2D
    {
        int x, y; // Pixels na tela
    };

    struct Vetor3
    {
        float x, y, z;
    };

    struct Matriz3x3
    {
        float m[3][3];
    };

}

namespace opalg
{
    estrutura::Vetor3 adicao(estrutura::Vetor3 a, estrutura::Vetor3 b);
    estrutura::Vetor3 subtracao(estrutura::Vetor3 a, estrutura::Vetor3 b);
    estrutura::Vetor3 multEscalar(float k, estrutura::Vetor3 a);
    estrutura::Vetor3 multMatrizVetor(estrutura::Matriz3x3 m, estrutura::Vetor3 v);
} // Operações Algebricas

namespace opesp
{
    float produtoEscalar(estrutura::Vetor3 a, estrutura::Vetor3 b);
    float norma(estrutura::Vetor3 a);
    estrutura::Vetor3 produtoVetorial(estrutura::Vetor3 a, estrutura::Vetor3 b);
} // Operações espaciais
