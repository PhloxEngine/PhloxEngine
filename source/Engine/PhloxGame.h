#pragma once
#include <string>

struct GameInfo {
    std::string title;
    std::string version;
    std::string author;
};

class PhloxGame {
public:
    PhloxGame();
    ~PhloxGame();

    bool LoadGame(const char* phloxFilePath);
    const GameInfo& GetGameInfo() const { return m_gameInfo; }

private:
    GameInfo m_gameInfo;
    bool ReadGameInfo(const char* data, size_t size);
}; 