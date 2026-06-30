//
// Created by valma on 6/22/2026.
//

#ifndef BUSQUEDAINGENUA_H
#define BUSQUEDAINGENUA_H

#include <string>
#include <vector>
#include <chrono>

class BusquedaIngenua {
public:

    std::vector<int> findOccurrences(const std::string& text, const std::string& pattern) {
        auto startTime = std::chrono::high_resolution_clock::now();

        std::vector<int> result;

        for (int i = 0; i <= (int)text.size() - (int)pattern.size(); i++) {
            bool match = true;
            for (int j = 0; j < (int)pattern.size(); j++) {
                if(text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }

            if(match) {
                result.push_back(i);
            }

        }
        auto endTime = std::chrono::high_resolution_clock::now();
        lastSearchTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();

        return result;
    }

    int countOccurrences(const std::string& text, const std::string& pattern) {
        return findOccurrences(text, pattern).size();
    }

    double getLastSearchTimeMs() const { return lastSearchTimeMs; }

private:
    double lastSearchTimeMs = 0.0;
};

#endif //BUSQUEDAINGENUA_H
