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
    SuffixTreeNode* children[256];   // indexado por char (ASCII)
    SuffixTreeNode* suffixLink;

    // Sufijo (solo hojas)
    int suffixIndex;                 // -1 si es nodo interno

    // Constructor
    SuffixTreeNode(int start, int* end);
    // Destructor
    ~SuffixTreeNode();

    // Métodos
    int  edgeLength() const;         // end - start + 1
    bool isLeaf()     const;         // suffixIndex != -1
};

#endif //SUFFIXTREENODE_H
