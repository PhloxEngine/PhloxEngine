#pragma once
#include <string>
#include "StateManager.h"
#include "ScriptHandler.h"

struct WindowConfig {
    int width = 800;
    int height = 600;
};

struct GameInfo {
    std::string title;
    std::string version;
    std::string author;
    WindowConfig window;
    std::string initialState;
};

class PhloxGame {
public:
    PhloxGame();
    ~PhloxGame();

    bool LoadGame(const char* phloxFilePath);
    void Update();
    void Render();
    
    bool LoadInitialState(const std::string& stateName);
    const GameInfo& GetGameInfo() const { return m_gameInfo; }

private:
    GameInfo m_gameInfo;
    StateManager& m_stateManager = StateManager::GetInstance();
    ScriptHandler& m_scriptHandler = ScriptHandler::GetInstance();
    
    bool ReadGameInfo(const char* data, size_t size);
    bool LoadScripts(std::ifstream& file);
}; 