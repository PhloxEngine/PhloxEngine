#include "PhloxGame.h"
#include <fstream>
#include <vector>
#include "json.hpp"
#include "ScriptParser.h"
#include "StateManager.h"
#include <cstring>
#include <iostream>  
#include "ScriptableSprite.h"

PhloxGame::PhloxGame() {
    ScriptParser::GetInstance().RegisterNativeClass("State");
}

PhloxGame::~PhloxGame() {}

bool PhloxGame::LoadGame(const char* phloxFilePath) {
    std::ifstream file(phloxFilePath, std::ios::binary);
    if (!file.is_open()) {
        char errMsg[256];
        strerror_s(errMsg, sizeof(errMsg), errno);
        std::cerr << "Failed to open " << phloxFilePath << ": " << errMsg << std::endl;
        return false;
    }

    uint32_t gameInfoSize;
    if (!file.read(reinterpret_cast<char*>(&gameInfoSize), sizeof(gameInfoSize))) {
        std::cerr << "Failed to read game info size" << std::endl;
        return false;
    }
    
    std::vector<char> gameInfoData(gameInfoSize);
    if (!file.read(gameInfoData.data(), gameInfoSize)) {
        std::cerr << "Failed to read game info data" << std::endl;
        return false;
    }
    
    if (!ReadGameInfo(gameInfoData.data(), gameInfoSize)) {
        std::cerr << "Failed to parse game info" << std::endl;
        return false;
    }

    if (!LoadScripts(file)) {
        std::cerr << "Failed to load scripts" << std::endl;
        return false;
    }

    class DynamicState : public State {
        std::string stateName;
    public:
        DynamicState(const std::string& name) : stateName(name) {}
        void Create() override { ScriptParser::GetInstance().ExecuteFunction(stateName, "create"); }
        void Update() override { ScriptParser::GetInstance().ExecuteFunction(stateName, "update"); }
        void Render() override { ScriptParser::GetInstance().ExecuteFunction(stateName, "render"); }
        void Destroy() override { ScriptParser::GetInstance().ExecuteFunction(stateName, "destroy"); }
    };

    StateManager::GetInstance().PushState(std::make_unique<DynamicState>(m_gameInfo.initialState));
    return true;
}

bool PhloxGame::ReadGameInfo(const char* data, size_t size) {
    try {
        nlohmann::json j = nlohmann::json::parse(data, data + size);
        m_gameInfo.title = j["title"].get<std::string>();
        m_gameInfo.version = j["version"].get<std::string>();
        m_gameInfo.author = j["author"].get<std::string>();
        m_gameInfo.window.width = j["window"]["width"].get<int>();
        m_gameInfo.window.height = j["window"]["height"].get<int>();
        m_gameInfo.initialState = j["initialState"].get<std::string>();
        return true;
    }
    catch (...) {
        return false;
    }
}

bool PhloxGame::LoadScripts(std::ifstream& file) {
    while (file.peek() != EOF) {
        uint32_t filenameSize;
        if (!file.read(reinterpret_cast<char*>(&filenameSize), sizeof(filenameSize))) {
            std::cerr << "Failed to read filename size" << std::endl;
            return false;
        }
        
        std::vector<char> filename(filenameSize + 1);
        if (!file.read(filename.data(), filenameSize)) {
            std::cerr << "Failed to read filename" << std::endl;
            return false;
        }
        filename[filenameSize] = '\0';
        
        uint32_t scriptSize;
        if (!file.read(reinterpret_cast<char*>(&scriptSize), sizeof(scriptSize))) {
            std::cerr << "Failed to read script size" << std::endl;
            return false;
        }
        
        std::vector<char> scriptData(scriptSize + 1);
        if (!file.read(scriptData.data(), scriptSize)) {
            std::cerr << "Failed to read script data" << std::endl;
            return false;
        }
        scriptData[scriptSize] = '\0';
        
        std::cout << "Loading script: " << filename.data() << std::endl;
        if (!ScriptParser::GetInstance().ParseFile(scriptData.data())) {
            std::cerr << "Failed to parse script: " << filename.data() << std::endl;
            return false;
        }
    }
    return true;
}

void PhloxGame::Update() {
    StateManager::GetInstance().Update();
}

void PhloxGame::Render() {
    StateManager::GetInstance().Render();
    ScriptableSprite::RenderAll();
} 