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
    ~SuffixTree() {
        freeDFS(root);
    }

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
    bool contains(const std::string& pattern) {
        return findNode(pattern)!= nullptr;
    }
    int countOccurrences(const std::string& pattern) {
        return findOccurrences(pattern).size();
    }
    std::vector<int> findOccurrences(const std::string& pattern) {
        auto startTime = std::chrono::high_resolution_clock::now();

        std::vector<int> result;

        SuffixTreeNode* node = findNode(pattern);

        if(node!=nullptr) {
            collectLeaves(node, result);
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        lastSearchTimeMs = std::chrono::duration<double, std::milli>(endTim - startTime).count();

        return result;
    }

    //Métricas
    double getBuildTimeMs() const {return buildTimeMs;}
    double getLastSearchTimeMs() const {return lastSearchTimeMs;}

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

            if(activeNode->children[(unsigned char)text[activeEdge]] == nullptr) {
                activeNode->children[(unsigned char)text[activeEdge]] = newNode(pos, &globalEnd);

                if (lastNewInternal != nullptr) {
                    lastNewInternal->suffixLink = activeNode;
                    lastNewInternal = nullptr;
                }
            }
            else {

                SuffixTreeNode* next = activeNode->children[(unsigned char)text[activeEdge]];

                if(walkDown(next)) continue;

                if(text[next->start + activeLength] == text[pos]) {
                    activeLength++;
                    if (lastNewInternal != nullptr) {
                        lastNewInternal->suffixLink = activeNode;
                        lastNewInternal = nullptr;
                    }
                    break;
                }

                int* splitEnd = new int(next->start + activeLength - 1);
                SuffixTreeNode* split = newNode(next->start, splitEnd);

                activeNode->children[(unsigned char)text[activeEdge]] = split;
                split->children[(unsigned char)text[pos]] = newNode(pos, &globalEnd);
                next->start += activeLength;
                split->children[(unsigned char)text[next->start]] = next;

                if (lastNewInternal != nullptr)
                    lastNewInternal->suffixLink = split;
                lastNewInternal = split;

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
    void setSuffixIndexDFS(SuffixTreeNode* node, int labelHeight) {
        if (node == nullptr) return;

        bool isLeaf = true;

        for(int i = 0; i < 256; i++) {
            if (node->children[i] != nullptr) {
                isLeaf = false;
                setSuffixIndexDFS(
                    node->children[i],
                    labelHeight + node->children[i]->edgeLength()
                );
            }
        }

        if (isLeaf) {
            node->suffixIndex = n - labelHeight;
        }
    }

    SuffixTreeNode*  findNode(const std::string& pattern) const {
        SuffixTreeNode* node = root;
        int i = 0;
        while (i < pattern.size()) {
            int idx = SuffixTreeNode::charIndex(pattern[i]);

            if(node->children[idx]==nullptr) return nullptr;

            SuffixTreeNode* child = node->children[idx];

            int edgeStart = child->start;
            int edgeEnd = *child->end;

            for(int j = edgeStart; j<= edgeEnd && i <pattern.size(); j++, i++) {
                if(text[j] != pattern[i])
                    return nullptr;
            }
            node = child;
        }

        return node;
    }  // retorna nodo donde termina el patrón


    void collectLeaves(SuffixTreeNode* node, std::vector<int>& result) const {
        if (node == nullptr) return;

        if(node->isLeaf()) {
            result.push_back(node->suffixIndex);
            return;
        }

        for(int i = 0; i<27; i++) {
            if (node->children[i] != nullptr) {
                collectLeaves(node->children[i], result);
            }
        }
    }// DFS recolecta hojas
    void freeDFS(SuffixTreeNode* node) {
        if (node == nullptr) return;

        for(int i = 0; i < 256; i++) {
            if (node->children[i] != nullptr)
                freeDFS(node->children[i]);
        }

        delete node;
    }
};

#endif //SUFFIXTREE_H
