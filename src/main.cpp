#include "Engine/Application.h"

int main(int argc, char** argv) {
    auto& app = Application::getInstance();
    
    app.setWindowSize(1280, 720);
    app.setTitle("Phlox Engine");
    
    app.init(argc, argv);
    app.run();
    
    return 0;
}
