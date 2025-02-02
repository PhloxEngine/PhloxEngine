#pragma once
#include "PhloxGame.h"
#include <GL/freeglut.h>

class Application {
public:
    Application();
    ~Application();

    bool Initialize(int argc, char* argv[]);
    bool LoadGame(const char* phloxFilePath);
    void Update();
    void Render();
    void Cleanup();
    bool IsRunning() const;
    
    static Application& GetInstance() { return *s_instance; }
    static void DisplayCallback();
    static void IdleCallback();
    static void ReshapeCallback(int width, int height);
    static void KeyboardCallback(unsigned char key, int x, int y);
    static void CloseCallback();

private:
    void UpdateWindowTitle();
    void UpdateDiscordPresence();

    int m_windowId;
    bool m_isRunning;
    PhloxGame m_game;
    
    static Application* s_instance;
    friend class State;
};