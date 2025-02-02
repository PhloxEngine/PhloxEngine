#pragma once
#include <string>
#include "StateManager.h"
#include "ScriptParser.h"

struct WindowConfig {
    int width;
    int height;
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
    const GameInfo& GetGameInfo() const { return m_gameInfo; }
    void Update();
    void Render();

private:
    GameInfo m_gameInfo;
    bool ReadGameInfo(const char* data, size_t size);
    bool LoadScripts(std::ifstream& file);
}; 