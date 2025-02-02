#pragma once
#include <iostream>
#include <string>

namespace Debug {
    inline void Trace(const std::string& message) {
        std::cout << "[PhloxEngine] " << message << std::endl;
    }
    
    inline void Error(const std::string& message) {
        std::cerr << "[PhloxEngine Error] " << message << std::endl;
    }
}

inline void trace(const std::string& message) {
    Debug::Trace(message);
} 