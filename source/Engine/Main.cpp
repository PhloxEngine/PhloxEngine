#include "Application.h"

int main(int argc, char* argv[]) {
    Application app;
    
    if (!app.Initialize(argc, argv)) {
        return -1;
    }

    app.LoadGame("Data.phlox");

    glutMainLoop();

    app.Cleanup();
    return 0;
}