#include "../include/AlgLinear.h"
#include <math.h>

// Operações Algébricas

estrutura::Vetor3 opalg::adicao(estrutura::Vetor3 a, estrutura::Vetor3 b)
{
    return estrutura::Vetor3{a.x + b.x, a.y + b.y, a.z + b.z};
}

estrutura::Vetor3 opalg::subtracao(estrutura::Vetor3 a, estrutura::Vetor3 b)
{
    return estrutura::Vetor3{a.x - b.x, a.y - b.y, a.z - b.z};
}

estrutura::Vetor3 opalg::multEscalar(float k, estrutura::Vetor3 a)
{
    return estrutura::Vetor3{k * a.x, k * a.y, k * a.z};
}

estrutura::Vetor3 opalg::multMatrizVetor(estrutura::Matriz3x3 m, estrutura::Vetor3 v)
{
    estrutura::Vetor3 resultado;

    resultado.x = (m.m[0][0] * v.x) + (m.m[0][1] * v.y) + (m.m[0][2] * v.z);
    resultado.y = (m.m[1][0] * v.x) + (m.m[1][1] * v.y) + (m.m[1][2] * v.z);
    resultado.z = (m.m[2][0] * v.x) + (m.m[2][1] * v.y) + (m.m[2][2] * v.z);

    return resultado;
}

// Operações Espaciais

float opesp::produtoEscalar(estrutura::Vetor3 a, estrutura::Vetor3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float opesp::norma(estrutura::Vetor3 a)
{
    // A norma é a raiz quadrada do produto escalar do vetor por ele mesmo
    return std::sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

estrutura::Vetor3 opesp::produtoVetorial(estrutura::Vetor3 a, estrutura::Vetor3 b)
{
    return estrutura::Vetor3{
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)};
}