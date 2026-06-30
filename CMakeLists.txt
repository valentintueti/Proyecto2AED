cmake_minimum_required(VERSION 3.28)
project(Proyecto2_AED_SuffixTree)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto2_AED_SuffixTree src/main.cpp
        src/core/SuffixTree.h
        src/core/BusquedaIngenua.h
        src/core/suffixTreeNode.h
        src/loader/textLoader.h
        src/loader/textNormalizer.h
        src/ui/app.h)

# SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(Proyecto2_AED_SuffixTree PRIVATE sfml-graphics sfml-window sfml-system)