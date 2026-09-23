cmake_minimum_required(VERSION 3.10) 
project(computacao_grafica) 

set(CMAKE_CXX_STANDARD 20)

add_executable(computacao_grafica main.cpp
    src/AlgLinear.cpp
    include/AlgLinear.h
    src/Utils.cpp
    include/Utils.h
    src/Camera.cpp
    include/Camera.h
    src/Rasteirizador.cpp 
    include/Rasteirizador.h 
)