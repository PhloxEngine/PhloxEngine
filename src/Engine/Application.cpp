#include "Application.h"

Application* Application::instance = nullptr;

Application::Application() 
    : windowWidth(800)
    , windowHeight(600)
    , title("Phlox Engine") 
{
}

Application& Application::getInstance() {
    if (!instance) {
        instance = new Application();
    }
    return *instance;
}

void Application::init(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(title.c_str());
        
    glutDisplayFunc(renderCallback);
    glutTimerFunc(16, updateCallback, 0); // ~60 FPS
    
    engine.loadScript("main.lua");
}

void Application::renderCallback() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, instance->windowWidth, instance->windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    
    lua_State* L = instance->engine.getLuaState();
    lua_getglobal(L, "render");
    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            std::cerr << "Error in render: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);  
        }
    } else {
        lua_pop(L, 1); 
    }
    
    glutSwapBuffers();
}

void Application::updateCallback(int value) {
    instance->engine.update(0.016f);
    glutPostRedisplay();
    glutTimerFunc(16, updateCallback, 0);
}

void Application::run() {
    glutMainLoop();
}

void Application::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    if (glutGet(GLUT_INIT_STATE)) {
        glutReshapeWindow(width, height);
    }
}

void Application::setTitle(const std::string& windowTitle) {
    title = windowTitle;
    if (glutGet(GLUT_INIT_STATE)) {
        glutSetWindowTitle(title.c_str());
    }
} 