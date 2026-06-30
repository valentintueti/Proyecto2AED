//
// Created by valma on 6/22/2026.
//

#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>
#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include "../core/suffixTreeNode.h"
#include "../core/SuffixTree.h"
#include "../core/BusquedaIngenua.h"
#include "../loader/textLoader.h"
#include "../loader/textNormalizer.h"

class App {
public:
    App() : window(sf::VideoMode(1280, 720), "Suffix Tree - Buscador de Documentos") {
        window.setFramerateLimit(60);
        if (!font.loadFromFile("assets/fonts/OpenSans.ttf"))
            exit(-1);
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            render();
        }
    }

private:
    // ── Ventana ─────────────────────────────────────────
    sf::RenderWindow window;
    sf::Font         font;

    // ── Estado ──────────────────────────────────────────
    bool        fileLoaded    = false;
    bool        searched      = false;
    bool        typingSearch  = false;
    std::string searchQuery;
    std::string filePath;

    // ── Datos ───────────────────────────────────────────
    TextLoader      loader;
    TextNormalizer  normalizer;
    SuffixTree      tree;
    BusquedaIngenua naive;

    // ── Resultados ──────────────────────────────────────
    std::vector<int>         occurrences;
    std::vector<int>         naiveResults;
    std::vector<std::string> patternPath;
    float                    scrollOffset = 0.f;

    // ── Colores ─────────────────────────────────────────
    sf::Color colorBg        = sf::Color(30, 30, 30);
    sf::Color colorPanel     = sf::Color(45, 45, 45);
    sf::Color colorAccent    = sf::Color(100, 180, 255);
    sf::Color colorText      = sf::Color(220, 220, 220);
    sf::Color colorHighlight = sf::Color(255, 200, 0);
    sf::Color colorButton    = sf::Color(60, 60, 60);
    sf::Color colorSuccess   = sf::Color(100, 220, 100);

    // ────────────────────────────────────────────────────
    // Explorador de archivos de Windows
    // ────────────────────────────────────────────────────
    std::string openFileDialog() {
        char filename[MAX_PATH] = "";
        OPENFILENAMEA ofn;
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize  = sizeof(ofn);
        ofn.hwndOwner    = window.getSystemHandle();
        ofn.lpstrFilter  = "Text Files\0*.txt\0All Files\0*.*\0";
        ofn.lpstrFile    = filename;
        ofn.nMaxFile     = MAX_PATH;
        ofn.lpstrTitle   = "Seleccionar archivo de texto";
        ofn.Flags        = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (GetOpenFileNameA(&ofn))
            return std::string(filename);
        return "";
    }

    // ────────────────────────────────────────────────────
    // Cargar archivo y construir árbol
    // ────────────────────────────────────────────────────
    void loadFile(const std::string& path) {
        loader.loadTXT(path);
        if (!loader.success) return;

        normalizer.normalize(loader.rawText);
        tree.buildUkkonen(normalizer.normalized);

        filePath   = path;
        fileLoaded = true;
        searched   = false;
        occurrences.clear();
        naiveResults.clear();
        patternPath.clear();
        searchQuery.clear();
        scrollOffset = 0.f;
    }

    // ────────────────────────────────────────────────────
    // Buscar patrón
    // ────────────────────────────────────────────────────
    void search(const std::string& pattern) {
        if (!fileLoaded || pattern.empty()) return;

        // normalizar el patrón igual que el texto
        std::string normPattern = pattern;
        for (char& c : normPattern) {
            c = tolower(c);
            if (c < 'a' || c > 'z') c = ' ';
        }

        occurrences  = tree.findOccurrences(normPattern);
        naiveResults = naive.findOccurrences(normalizer.normalized, normPattern);
        patternPath  = tree.getPatternPath(normPattern);
        searched     = true;
        scrollOffset = 0.f;
    }

    // ────────────────────────────────────────────────────
    // Helpers de dibujo
    // ────────────────────────────────────────────────────
    void drawRect(float x, float y, float w, float h, sf::Color color) {
        sf::RectangleShape rect(sf::Vector2f(w, h));
        rect.setPosition(x, y);
        rect.setFillColor(color);
        window.draw(rect);
    }

    void drawText(const std::string& str, float x, float y,
                  unsigned int size, sf::Color color) {
        sf::Text text(str, font, size);
        text.setPosition(x, y);
        text.setFillColor(color);
        window.draw(text);
    }

    void drawButton(const std::string& label, float x, float y,
                    float w, float h, sf::Color bg) {
        drawRect(x, y, w, h, bg);
        // centrar texto en el botón
        sf::Text text(label, font, 14);
        float tx = x + (w - text.getLocalBounds().width)  / 2.f;
        float ty = y + (h - text.getLocalBounds().height) / 2.f - 3.f;
        text.setPosition(tx, ty);
        text.setFillColor(colorText);
        window.draw(text);
    }

    bool isMouseOver(float x, float y, float w, float h) {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        return mouse.x >= x && mouse.x <= x + w &&
               mouse.y >= y && mouse.y <= y + h;
    }

    // ────────────────────────────────────────────────────
    // Obtener fragmento del texto original alrededor de pos
    // ────────────────────────────────────────────────────
    std::string getFragment(int pos, int radius = 40) {
        int start = std::max(0, pos - radius);
        int end   = std::min((int)normalizer.original.size(), pos + (int)searchQuery.size() + radius);
        std::string fragment = "..." + normalizer.original.substr(start, end - start) + "...";
        return fragment;
    }

    // ────────────────────────────────────────────────────
    // Eventos
    // ────────────────────────────────────────────────────
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            // scroll en el panel de fragmentos
            if (event.type == sf::Event::MouseWheelScrolled)
                scrollOffset -= event.mouseWheelScroll.delta * 20.f;

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                // botón cargar archivo (corregido: y=70, coincide con el render)
                if (isMouseOver(20, 70, 160, 36)) {
                    std::string path = openFileDialog();
                    if (!path.empty()) loadFile(path);
                }

                // botón buscar (corregido: y=130)
                if (fileLoaded && isMouseOver(900, 130, 100, 32))
                    search(searchQuery);

                // click en barra de búsqueda (corregido: y=130)
                if (fileLoaded && isMouseOver(420, 130, 470, 32))
                    typingSearch = true;
                else
                    typingSearch = false;
                }

            // tipeo en barra de búsqueda
            if (typingSearch && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!searchQuery.empty())
                        searchQuery.pop_back();
                } else if (event.text.unicode == '\r') {
                    search(searchQuery);
                } else if (event.text.unicode < 128) {
                    searchQuery += static_cast<char>(event.text.unicode);
                }
            }
        }
    }

    // ────────────────────────────────────────────────────
    // Render
    // ────────────────────────────────────────────────────
    void render() {
        window.clear(colorBg);

        // ── Header ──────────────────────────────────────
        drawRect(0, 0, 1280, 60, colorPanel);
        drawText("Suffix Tree - Buscador de Documentos", 20, 15, 18, colorAccent);

        // ── Botón cargar archivo ────────────────────────
        sf::Color btnColor = isMouseOver(20, 20, 160, 36) ?
                             sf::Color(80, 80, 80) : colorButton;
        drawButton("Cargar archivo .txt", 20, 70, 160, 36, btnColor);

        // ── Archivo cargado ─────────────────────────────
        if (fileLoaded) {
            drawText("Archivo: " + filePath, 200, 78, 13, colorSuccess);
            drawText("Construccion: " + std::to_string(tree.getBuildTimeMs()) + " ms",
                     200, 95, 12, colorText);
        }

        if (!fileLoaded) {
            drawText("Carga un archivo .txt para comenzar.", 400, 340, 16, colorText);
            window.display();
            return;
        }

        // ── Barra de búsqueda ───────────────────────────
        drawRect(0, 120, 1280, 1, sf::Color(70, 70, 70));  // separador
        drawText("Buscar:", 20, 138, 14, colorText);
        drawRect(420, 130, 470, 32, typingSearch ?
                 sf::Color(60, 60, 80) : sf::Color(50, 50, 50));
        drawText(searchQuery + (typingSearch ? "|" : ""), 425, 136, 14, colorText);

        sf::Color btnSearch = isMouseOver(900, 130, 100, 32) ?
                              sf::Color(80, 130, 200) : colorAccent;
        drawButton("Buscar", 900, 130, 100, 32, btnSearch);

        if (!searched) {
            drawText("Escribe un patron y presiona Buscar.", 400, 340, 16, colorText);
            window.display();
            return;
        }

        // ── Layout de paneles ───────────────────────────
        // Panel izquierdo: fragmentos (x=0,   w=640)
        // Panel derecho:   stats+tree (x=650, w=620)
        drawRect(0,   170, 640, 550, colorPanel);
        drawRect(650, 170, 620, 550, colorPanel);
        drawRect(640, 170, 10,  550, colorBg);  // separador

        // ── Panel izquierdo: Fragmentos ─────────────────
        drawText("Fragmentos (" + std::to_string(occurrences.size()) + " ocurrencias)",
                 10, 175, 14, colorAccent);

        float fy = 200.f - scrollOffset;
        for (int i = 0; i < (int)occurrences.size() && i < 30; i++) {
            if (fy > 165.f && fy < 710.f) {
                // posición
                drawText("Pos " + std::to_string(occurrences[i]) + ":",
                         10, fy, 12, colorHighlight);
                // fragmento
                std::string frag = getFragment(occurrences[i]);
                // truncar si es muy largo
                if (frag.size() > 70) frag = frag.substr(0, 70) + "...";
                drawText(frag, 80, fy, 12, colorText);
            }
            fy += 22.f;
        }

        // ── Panel derecho arriba: Stats ──────────────────
        drawText("Patron buscado: \"" + searchQuery + "\"", 660, 175, 14, colorAccent);
        drawText("Total ocurrencias (SuffixTree): " +
                 std::to_string(occurrences.size()), 660, 200, 13, colorText);
        drawText("Total ocurrencias (Naive):      " +
                 std::to_string(naiveResults.size()), 660, 218, 13, colorText);

        drawRect(650, 240, 620, 1, sf::Color(70, 70, 70));  // separador

        drawText("Tiempos:", 660, 248, 14, colorAccent);
        drawText("Construccion del arbol:  " +
                 std::to_string(tree.getBuildTimeMs())       + " ms", 660, 268, 13, colorText);
        drawText("Busqueda Suffix Tree:    " +
                 std::to_string(tree.getLastSearchTimeMs())  + " ms", 660, 286, 13, colorText);
        drawText("Busqueda Naive:          " +
                 std::to_string(naive.getLastSearchTimeMs()) + " ms", 660, 304, 13, colorText);

        drawRect(650, 325, 620, 1, sf::Color(70, 70, 70));  // separador

        // ── Panel derecho abajo: Ruta en el árbol ────────
        drawText("Ruta en el Suffix Tree:", 660, 333, 14, colorAccent);

        std::string pathStr = "";
        for (int i = 0; i < (int)patternPath.size(); i++) {
            pathStr += patternPath[i];
            if (i < (int)patternPath.size() - 1)
                pathStr += " -> ";
        }
        // truncar si es muy largo
        if (pathStr.size() > 60) pathStr = pathStr.substr(0, 60) + "...";
        drawText(pathStr, 660, 358, 13, colorHighlight);

        drawRect(650, 380, 620, 1, sf::Color(70, 70, 70));  // separador

        // ── Posiciones listadas ──────────────────────────
        drawText("Posiciones:", 660, 388, 14, colorAccent);
        std::string posStr = "";
        for (int i = 0; i < (int)occurrences.size() && i < 20; i++) {
            posStr += std::to_string(occurrences[i]);
            if (i < (int)occurrences.size() - 1) posStr += ", ";
        }
        if (occurrences.size() > 20) posStr += "...";

        // partir en líneas de 60 chars
        float py = 408.f;
        while (posStr.size() > 60) {
            drawText(posStr.substr(0, 60), 660, py, 12, colorText);
            posStr = posStr.substr(60);
            py += 18.f;
        }
        drawText(posStr, 660, py, 12, colorText);

        window.display();
    }
};

#endif //APP_H
