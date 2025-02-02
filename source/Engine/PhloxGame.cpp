#include "PhloxGame.h"
#include "Debug.h"
#include <fstream>
#include <vector>
#include "json.hpp"
#include "ScriptInterpreter.h"

PhloxGame::PhloxGame() {}

PhloxGame::~PhloxGame() {}

bool PhloxGame::LoadGame(const char* phloxFilePath) {
    Debug::Trace("Loading game from: " + std::string(phloxFilePath));
    
    std::ifstream file(phloxFilePath, std::ios::binary);
    if (!file.is_open()) {
        Debug::Error("Failed to open file: " + std::string(phloxFilePath));
        return false;
    }

    uint32_t gameInfoSize;
    file.read(reinterpret_cast<char*>(&gameInfoSize), sizeof(gameInfoSize));
    
    std::vector<char> gameInfoData(gameInfoSize);
    file.read(gameInfoData.data(), gameInfoSize);
    
    if (!ReadGameInfo(gameInfoData.data(), gameInfoSize)) {
        Debug::Error("Failed to read game info");
        return false;
    }

    Debug::Trace("Loaded game info: " + m_gameInfo.title + " v" + m_gameInfo.version);

    while (file.peek() != EOF) {
        uint32_t filenameSize;
        file.read(reinterpret_cast<char*>(&filenameSize), sizeof(filenameSize));
        
        std::vector<char> filename(filenameSize + 1, 0);
        file.read(filename.data(), filenameSize);
        
        uint32_t scriptSize;
        file.read(reinterpret_cast<char*>(&scriptSize), sizeof(scriptSize));
        
        std::vector<char> scriptData(scriptSize);
        file.read(scriptData.data(), scriptSize);

        std::string scriptName(filename.data());
        if (!m_scriptHandler.LoadScript(scriptName, scriptData.data(), scriptSize)) {
            Debug::Error("Failed to load script: " + scriptName);
            return false;
        }
        Debug::Trace("Loaded script: " + scriptName);
    }

    if (m_gameInfo.initialState.empty()) {
        Debug::Error("No initial state specified in game info");
        return false;
    }

    if (!LoadInitialState(m_gameInfo.initialState)) {
        Debug::Error("Failed to load initial state: " + m_gameInfo.initialState);
        return false;
    }

    Debug::Trace("Game loaded successfully!");
    return true;
}

bool PhloxGame::LoadInitialState(const std::string& stateName) {
    Debug::Trace("Loading initial state: " + stateName);
    
    try {
        const auto& script = m_scriptHandler.GetScript(stateName);
        if (script.empty()) {
            Debug::Error("Script not found: " + stateName);
            return false;
        }
        
        auto state = ScriptInterpreter::GetInstance().CreateStateFromScript(script);
        if (!state) {
            Debug::Error("Failed to create state from script");
            return false;
        }
        
        m_stateManager.Switch(std::move(state));
        return true;
    }
    catch (const std::exception& e) {
        Debug::Error("Failed to load initial state: " + std::string(e.what()));
        return false;
    }
}

void PhloxGame::Update() {
    m_stateManager.Update(1.0f / 60.0f);
}

void PhloxGame::Render() {
    m_stateManager.Render();
}

bool PhloxGame::ReadGameInfo(const char* data, size_t size) {
    try {
        nlohmann::json j = nlohmann::json::parse(data, data + size);
        m_gameInfo.title = j["title"].get<std::string>();
        m_gameInfo.version = j["version"].get<std::string>();
        m_gameInfo.author = j["author"].get<std::string>();
        m_gameInfo.initialState = j["initialState"].get<std::string>();
        
        if (j.contains("window")) {
            auto& window = j["window"];
            if (window.contains("width")) {
                m_gameInfo.window.width = window["width"].get<int>();
            }
            if (window.contains("height")) {
                m_gameInfo.window.height = window["height"].get<int>();
            }
        }

        if (j.contains("discord")) {
            auto& discord = j["discord"];
            if (discord.contains("enabled")) {
                m_gameInfo.discord.enabled = discord["enabled"].get<bool>();
            }
            if (discord.contains("applicationId")) {
                m_gameInfo.discord.applicationId = discord["applicationId"].get<std::string>();
            }
            if (discord.contains("state")) {
                m_gameInfo.discord.state = discord["state"].get<std::string>();
            }
            if (discord.contains("details")) {
                m_gameInfo.discord.details = discord["details"].get<std::string>();
            }
            if (discord.contains("largeImageKey")) {
                m_gameInfo.discord.largeImageKey = discord["largeImageKey"].get<std::string>();
            }
            if (discord.contains("largeImageText")) {
                m_gameInfo.discord.largeImageText = discord["largeImageText"].get<std::string>();
            }
            if (discord.contains("smallImageKey")) {
                m_gameInfo.discord.smallImageKey = discord["smallImageKey"].get<std::string>();
            }
            if (discord.contains("smallImageText")) {
                m_gameInfo.discord.smallImageText = discord["smallImageText"].get<std::string>();
            }
        }
        return true;
    }
    catch (const std::exception& e) {
        Debug::Error("Failed to parse game info: " + std::string(e.what()));
        return false;
    }
} 