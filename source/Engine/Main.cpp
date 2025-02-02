#include "Application.h"

int main(int argc, char* argv[]) {
    Application app;
    
    if (!app.Initialize()) {
        return -1;
    }

    while (app.IsRunning()) {
        app.HandleEvents();
        app.Update();
        app.Render();
    }

    app.Cleanup();
    return 0;
}