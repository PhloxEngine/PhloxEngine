#include "PhloxGame.h"
#include <fstream>
#include <vector>
#include "json.hpp"

PhloxGame::PhloxGame() {}

PhloxGame::~PhloxGame() {}

bool PhloxGame::LoadGame(const char* phloxFilePath) {
    std::ifstream file(phloxFilePath, std::ios::binary);
    if (!file.is_open()) return false;

    uint32_t gameInfoSize;
    file.read(reinterpret_cast<char*>(&gameInfoSize), sizeof(gameInfoSize));
    
    std::vector<char> gameInfoData(gameInfoSize);
    file.read(gameInfoData.data(), gameInfoSize);
    
    if (!ReadGameInfo(gameInfoData.data(), gameInfoSize)) {
        return false;
    }

    while (file.peek() != EOF) {
        uint32_t filenameSize;
        file.read(reinterpret_cast<char*>(&filenameSize), sizeof(filenameSize));
        
        file.seekg(filenameSize, std::ios::cur);
        
        uint32_t scriptSize;
        file.read(reinterpret_cast<char*>(&scriptSize), sizeof(scriptSize));
        file.seekg(scriptSize, std::ios::cur);
    }

    return true;
}

bool PhloxGame::ReadGameInfo(const char* data, size_t size) {
    try {
        nlohmann::json j = nlohmann::json::parse(data, data + size);
        m_gameInfo.title = j["title"].get<std::string>();
        m_gameInfo.version = j["version"].get<std::string>();
        m_gameInfo.author = j["author"].get<std::string>();        
        if (j.contains("window")) {
            auto& window = j["window"];
            if (window.contains("width")) {
                m_gameInfo.window.width = window["width"].get<int>();
            }
            if (window.contains("height")) {
                m_gameInfo.window.height = window["height"].get<int>();
            }
        }
        return true;
    }
    catch (...) {
        return false;
    }
}

void PhloxGame::Update() {} 