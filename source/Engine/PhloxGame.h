#pragma once
#include <string>

struct WindowConfig {
    int width = 800;
    int height = 600;
};

struct GameInfo {
    std::string title;
    std::string version;
    std::string author;
    WindowConfig window;
};

class PhloxGame {
public:
    PhloxGame();
    ~PhloxGame();

    bool LoadGame(const char* phloxFilePath);
    void Update();
    const GameInfo& GetGameInfo() const { return m_gameInfo; }

private:
    GameInfo m_gameInfo;
    bool ReadGameInfo(const char* data, size_t size);
}; 