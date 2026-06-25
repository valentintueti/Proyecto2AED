//
// Created by valma on 6/22/2026.
//

#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include <string>
#include <vector>
#include "SuffixTreeNode.h"
#include <chrono>

class SuffixTree {

public:
    //Constructor
    SuffixTree();

    //Destructor
    ~SuffixTree();

    //Métodos públicos
    void buildUkkonen(const std::string& inputText) {
        auto start_time = std::chrono::high_resolution_clock::now();

        text = inputText + "$";
        n = text.size();
        globalEnd = -1;
        remainder = 0;

        root = newNode(-1, new int(-1));
        root->suffixLink = root;

        activeNode = root;
        activeEdge = 0;
        activeLength = 0;

        for(int i = 0; i <n; i++) {
            extendTree(i);
        }

        setSuffixIndexDFS(root, 0);

        auto end_time = std::chrono::high_resolution_clock::now();

        buildTimeMs = std::chrono::duration<double, std::milli>(end_time - start_time);


    }
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
    SuffixTreeNode* newNode(int start, int* end) {
        return new SuffixTreeNode(start, end);
    }
    void extendTree(int pos) { // procesa un carácter (una fase)
        globalEnd++;
        remainder++;

        SuffixTreeNode* lastNewInternal = nullptr;

        while (remainder > 0) {
            if (activeLength == 0) activeEdge = pos;

            if(activeEnd->children[(unsigned char)text[activeEdge]] == nullptr) {
                activeNode->children[(unsigned char)text[activeEdge]] = newNode(pos, &globalEnd);

                if (lastNewInternal != nullptr) {
                    lastNewInternal->suffixLink = activeNode;
                    lastNewInternal = nullptr;
                }
            }
            else {

                SuffixTreeNode* next = activeNode->children[(unsigned char)text[activeEdge]];

                if(walkDown(next)) continue;

            }

            remainder--;
            if(activeNode == root && activeLength > 0) {
                activeLength--;
                activeEdge = pos - remainder + 1;
            } else if(activeNode->suffixLink != nullptr){
                activeNode = activeNode->suffixLink;
            } else {
                activeNode = root;
            }
        }
    }
    bool walkDown(SuffixTreeNode* node) { // ajusta active point al bajar
        if(activeLength >= node->edgeLength()) {
            activeEdge += node->edgeLength();
            activeLength -= node->edgeLength();
            activeNode = node;
            return true;
        }

        return false;
    }
    void             setSuffixIndexDFS(SuffixTreeNode* node, int labelHeight); // asigna suffixIndex a hojas

    SuffixTreeNode*  findNode(const std::string& pattern) const;  // retorna nodo donde termina el patrón
    void             collectLeaves(SuffixTreeNode* node, std::vector<int>& result) const; // DFS recolecta hojas
    void             freeDFS(SuffixTreeNode* node);    // libera memoria recursivamente
};

#endif //SUFFIXTREE_H
