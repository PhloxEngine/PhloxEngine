#pragma once
#include "PhloxGame.h"

struct SDL_Window;
struct SDL_Renderer;

class Application {
public:
    Application();
    ~Application();

    bool Initialize();
    bool LoadGame(const char* phloxFilePath);
    void HandleEvents();
    void Update();
    void Render();
    void Cleanup();
    bool IsRunning() const;

private:
    void UpdateWindowTitle();
    void UpdateDiscordPresence();

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_isRunning;
    PhloxGame m_game;
    
    static SDL_Renderer* GetRenderer() { return s_instance->m_renderer; }
    static Application* s_instance;
    friend class State; 
}; 