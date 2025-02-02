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
    SDL_Renderer* GetRenderer() const { return m_renderer; }
    
    static Application& GetInstance() { return *s_instance; }

private:
    void UpdateWindowTitle();
    void UpdateDiscordPresence();

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_isRunning;
    PhloxGame m_game;
    
    static Application* s_instance;
    friend class State; 
}; 