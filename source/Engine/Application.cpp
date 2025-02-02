#include "Application.h"
#include <string>
#include <chrono>
#include "Discord.h"
#include <iostream>

Application* Application::s_instance = nullptr;

Application::Application() 
    : m_windowId(-1)
    , m_isRunning(false)
{
    s_instance = this;
    Discord::GetInstance().Initialize();
}

Application::~Application() {
    Discord_Shutdown();
    Cleanup();
}

bool Application::Initialize(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    
    m_windowId = glutCreateWindow("Phlox Engine - No Game Loaded");
    
    glutDisplayFunc(DisplayCallback);
    glutIdleFunc(IdleCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutCloseFunc(CloseCallback);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    m_isRunning = true;
    return true;
}

void Application::DisplayCallback() {
    if (s_instance) {
        s_instance->Render();
    }
}

void Application::IdleCallback() {
    if (s_instance) {
        s_instance->Update();
        glutPostRedisplay();
    }
}

void Application::ReshapeCallback(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
}

void Application::KeyboardCallback(unsigned char key, int x, int y) {
    if (key == 27) { 
        if (s_instance) {
            s_instance->m_isRunning = false;
            glutLeaveMainLoop();
        }
    }
}

void Application::CloseCallback() {
    if (s_instance) {
        s_instance->m_isRunning = false;
    }
}

void Application::Update() {
    m_game.Update();
}

void Application::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
        
    glutSwapBuffers();
}

void Application::Cleanup() {
    if (m_windowId != -1) {
        glutDestroyWindow(m_windowId);
    }
}

bool Application::IsRunning() const {
    return m_isRunning;
}

bool Application::LoadGame(const char* phloxFilePath) {
    if (!m_game.LoadGame(phloxFilePath)) {
        std::cerr << "Failed to load game: " << phloxFilePath << std::endl;
        return false;
    }
    
    const WindowConfig& windowConfig = m_game.GetGameInfo().window;
    glutReshapeWindow(windowConfig.width, windowConfig.height);
    glutPositionWindow(
        (glutGet(GLUT_SCREEN_WIDTH) - windowConfig.width) / 2,
        (glutGet(GLUT_SCREEN_HEIGHT) - windowConfig.height) / 2
    );
    
    UpdateWindowTitle();
    UpdateDiscordPresence();
    return true;
}

void Application::UpdateWindowTitle() {
    std::string title = "Phlox Engine - " + m_game.GetGameInfo().title;
    glutSetWindowTitle(title.c_str());
}

void Application::UpdateDiscordPresence() {
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    
    discordPresence.state = "In Game";
    
    std::string details = m_game.GetGameInfo().title + " by " + m_game.GetGameInfo().author;
    discordPresence.details = details.c_str();
    
    auto now = std::chrono::system_clock::now();
    discordPresence.startTimestamp = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()
    ).count();
    
    discordPresence.largeImageKey = "icon";
    Discord_UpdatePresence(&discordPresence);
}