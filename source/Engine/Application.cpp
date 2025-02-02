#include "Application.h"
#include <SDL.h>
#include <string>

Application::Application() 
    : m_window(nullptr)
    , m_renderer(nullptr)
    , m_isRunning(false)
{
}

Application::~Application() {
    Cleanup();
}

bool Application::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    m_window = SDL_CreateWindow(
        "Phlox Engine - No Game Loaded",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (!m_window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    m_isRunning = true;
    return true;
}

void Application::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_isRunning = false;
        }
    }
}

void Application::Update() {}

void Application::Render() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    SDL_RenderPresent(m_renderer);
}

void Application::Cleanup() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
}

bool Application::IsRunning() const {
    return m_isRunning;
}

bool Application::LoadGame(const char* phloxFilePath) {
    if (!m_game.LoadGame(phloxFilePath)) {
        SDL_Log("Failed to load game: %s", phloxFilePath);
        return false;
    }
    UpdateWindowTitle();
    return true;
}

void Application::UpdateWindowTitle() {
    std::string title = "Phlox Engine - " + m_game.GetGameInfo().title;
    SDL_SetWindowTitle(m_window, title.c_str());
}