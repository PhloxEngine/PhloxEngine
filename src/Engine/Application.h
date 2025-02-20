#pragma once
#include "LuaGameEngine.h"
#include <GL/freeglut.h>
#include <string>
#include <iostream>

class Application {
private:
    static Application* instance;
    LuaGameEngine engine;
    
    int windowWidth;
    int windowHeight;
    std::string title;
    
    Application();

    static void renderCallback();
    static void updateCallback(int value);

public:
    static Application& getInstance();
    
    void init(int argc, char** argv);
    void run();
    void setWindowSize(int width, int height);
    void setTitle(const std::string& windowTitle);
    
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
}; 