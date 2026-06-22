//
// Created by valma on 6/22/2026.
//

#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include <string>
#include <vector>
#include "SuffixTreeNode.h"

class SuffixTree {

public:
    //Constructor
    SuffixTree();

    //Destructor
    ~SuffixTree();

    //Métodos públicos
    void             buildUkkonen(const std::string& text);
    bool             contains(const std::string& pattern) const;
    int              countOccurrences(const std::string& pattern) const;
    std::vector<int> findOccurrences(const std::string& pattern) const;

    //Métricas
    double getBuildTimeMs()      const;
    double getLastSearchTimeMs() const;

private:
    //Texto
    std::string text;
    int         n;

    //Nodos
    SuffixTreeNode* root;

    //Active Point
    SuffixTreeNode* activeNode;
    int             activeEdge;    // índice en text[] del primer char de la arista activa
    int             activeLength;

    //Remainder y globalEnd
    int remainder;
    int globalEnd;

    //Métricas
    double buildTimeMs;
    double lastSearchTimeMs;

    //Métodos internos
    SuffixTreeNode*  newNode(int start, int* end);
    void             extendTree(int pos);              // procesa un carácter (una fase)
    bool             walkDown(SuffixTreeNode* node);   // ajusta active point al bajar
    void             setSuffixIndexDFS(SuffixTreeNode* node, int labelHeight); // asigna suffixIndex a hojas

    SuffixTreeNode*  findNode(const std::string& pattern) const;  // retorna nodo donde termina el patrón
    void             collectLeaves(SuffixTreeNode* node, std::vector<int>& result) const; // DFS recolecta hojas
    void             freeDFS(SuffixTreeNode* node);    // libera memoria recursivamente
};

#endif //SUFFIXTREE_H
