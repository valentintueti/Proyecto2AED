cmake_minimum_required(VERSION 3.29)
project(Proyecto2_AED_SuffixTree)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto2_AED_SuffixTree src/main.cpp
        src/core/SuffixTree.h
        src/core/BusquedaIngenua.h
        src/loader/textLoader.h
        src/loader/textNormalizer.h
        src/ui/app.h)
