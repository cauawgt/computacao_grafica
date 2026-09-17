#include <iostream>
#include "include/Math3D.h"
#include "include/Utils.h"

int main()
{

    estrutura::Vetor3 v1 = {5, 7, 4};
    estrutura::Vetor3 v2 = {7, 6, 4};



    std::cout << "PROJETO COMPUTAÇÃO GRAFICA SETUP" << "\n";
    std::cout << visualizacao::vetor(v1);
    std::cout << visualizacao::vetor(opalg::adicao(v1, v2));
    std::cout << visualizacao::vetor(opalg::subtracao(v1, v2));
    std::cout << visualizacao::vetor(opalg::multEscalar(1.5, v1));

    return 0;
}