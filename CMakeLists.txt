cmake_minimum_required(VERSION 3.29)
project(IU_Compilulki_Compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(compilulki src/main.cpp
        src/settings/settings.cpp
        src/settings/settings.h)
