#include <sstream>
#include "../include/Utils.h"
#include "../include/AlgLinear.h"

std::string visualizacao::vetor(estrutura::Vetor3 v)
{
    std::stringstream ss;
    ss << "(" << v.x << ", " << v.y << ", " << v.z << ")" << "\n";
    return ss.str();
}