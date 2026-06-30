//
// Created by valma on 6/22/2026.
//

#ifndef SUFFIXTREENODE_H
#define SUFFIXTREENODE_H

struct SuffixTreeNode {

    // Arista
    int  start;
    int* end; // hojas comparten puntero globalEnd

    // Árbol
    SuffixTreeNode* children[28];   //
    SuffixTreeNode* suffixLink;

    // Sufijo (solo hojas)
    int suffixIndex;                 // -1 si es nodo interno

    // Constructor
    SuffixTreeNode(int start_, int* end_):
start(start_), end(end_), suffixLink(nullptr), suffixIndex(-1) {
        for(int i = 0; i < 28; i++) {
            children[i] = nullptr;
        }
    }
    // Destructor
    ~SuffixTreeNode() {
     if(!isLeaf()) delete end;
    }

    // Métodos
    int  edgeLength() const {
        return *end - start + 1;
    }

    bool isLeaf()     const {
        return suffixIndex != -1;
    }

    static int charIndex(char c) {
        if (c == ' ') return 26;
        if (c == '$') return 27;
        return c - 'a';
    }

};

#endif //SUFFIXTREENODE_H
