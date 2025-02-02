#pragma once

struct SDL_Window;
struct SDL_Renderer;

class Application {
public:
    Application();
    ~Application();

    bool Initialize();
    void HandleEvents();
    void Update();
    void Render();
    void Cleanup();
    bool IsRunning() const;

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_isRunning;
}; 