//
// Created by valma on 6/22/2026.
//

#ifndef TEXTNORMALIZER_H
#define TEXTNORMALIZER_H

#include <string>
#include <algorithm>

class TextNormalizer {
public:

    std::string original;

    std::string normalized;

    void normalize(const std::string& input) {
        original   = input;
        normalized = input;

        for (int i = 0; i < (int)normalized.size(); i++) {
            char c = tolower(normalized[i]);
            if (c >= 'a' && c <= 'z')
                normalized[i] = c;
            else
                normalized[i] = ' ';
        }
    }
};

#endif //TEXTNORMALIZER_H
