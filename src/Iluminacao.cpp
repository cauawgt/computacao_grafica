#include "../include/Iluminacao.h"
#include <cmath>
#include <algorithm>

estrutura::Vetor3 iluminacao::phong(estrutura::Vetor3 P,
                                    estrutura::Vetor3 N,
                                    const estrutura::Iluminacao &luz)
{
    using estrutura::Vetor3;

    // V = direção de vista (de P para o observador). Em coordenadas de vista
    // o foco é C = (0,0,0), logo V = C - P = -P. Normalizar.
    Vetor3 V = opesp::normalizar(opalg::multEscalar(-1.0f, P));

    // L = Pl - P (direção de P para a fonte de luz). Normalizar.
    Vetor3 L = opesp::normalizar(opalg::subtracao(luz.Pl, P));

    // Componente ambiente: Ia = Ka * Iamb
    Vetor3 Ia = opalg::multEscalar(luz.Ka, luz.Iamb);
    Vetor3 Id{0.0f, 0.0f, 0.0f};
    Vetor3 Is{0.0f, 0.0f, 0.0f};

    // casos especiais envolvendo <N,L>
    //   Se <N,L> < 0:
    //      Se <V,N> < 0  -> N <- -N
    //      Senão         -> desconsidera a fonte de luz (difuso e especular nulos)
    float NL = opesp::produtoEscalar(N, L);
    bool usaLuz = true;
    if (NL < 0.0f)
    {
        if (opesp::produtoEscalar(V, N) < 0.0f)
        {
            N = opalg::multEscalar(-1.0f, N);
            NL = -NL; // <N,L> com o novo N
        }
        else
        {
            usaLuz = false;
        }
    }

    if (usaLuz)
    {
        // R = 2<N,L>N - L (já normalizado, pois N e L são unitários).
        // Calculado depois do teste acima porque depende do N final.
        Vetor3 R = opalg::subtracao(opalg::multEscalar(2.0f * NL, N), L);

        // Difusa: Id = <N,L> * (Kd * Od * Il)   (* = componente a componente)
        Vetor3 KdOdIl{luz.Kd.x * luz.Od.x * luz.Il.x,
                      luz.Kd.y * luz.Od.y * luz.Il.y,
                      luz.Kd.z * luz.Od.z * luz.Il.z};
        Id = opalg::multEscalar(NL, KdOdIl);

        //se <V,R> < 0, desconsidera a componente especular.
        // Especular: Is = <R,V>^eta * Ks * Il
        float VR = opesp::produtoEscalar(V, R);
        if (VR >= 0.0f)
            Is = opalg::multEscalar(std::pow(VR, luz.eta) * luz.Ks, luz.Il);
    }

    // I = Ia + Id + Is, e se algum canal passar de 255, vira 255.
    Vetor3 I = opalg::adicao(opalg::adicao(Ia, Id), Is);
    I.x = std::min(I.x, 255.0f);
    I.y = std::min(I.y, 255.0f);
    I.z = std::min(I.z, 255.0f);
    return I;
}