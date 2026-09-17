#pragma once

namespace estrutura
{
    struct Vetor3
    {
        float x, y, z;
    };

}

namespace opalg
{
    estrutura::Vetor3 adicao(estrutura::Vetor3 a, estrutura::Vetor3 b);
    estrutura::Vetor3 subtracao(estrutura::Vetor3 a, estrutura::Vetor3 b);
    estrutura::Vetor3 multEscalar(float k, estrutura::Vetor3 a);
} // Operações Algebricas

namespace opesp
{
    estrutura::Vetor3 produtoEscalar(estrutura::Vetor3 a, estrutura::Vetor3 b);
    float norma(estrutura::Vetor3 a);
    estrutura::Vetor3 produtoVetorial(estrutura::Vetor3 a, estrutura::Vetor3 b);
} // Operações espaciais
