//
// Created by valma on 6/22/2026.
//

#ifndef TEXTLOADER_H
#define TEXTLOADER_H

#include <string>
#include <fstream>
#include <sstream>

class TextLoader {
public:


    std::string rawText;
    bool        success = false;
    std::string errorMsg;

    void loadTXT(const std::string& filepath) {
        std::ifstream file(filepath);

        if (!file.is_open()) {
            success  = false;
            errorMsg = "No se pudo abrir el archivo: " + filepath;
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        rawText = buffer.str();
        success = true;

        file.close();
    }
};

#endif //TEXTLOADER_H
