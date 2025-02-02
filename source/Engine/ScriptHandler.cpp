#include "ScriptHandler.h"
#include "Debug.h"
#include <filesystem>

bool ScriptHandler::LoadScript(const std::string& name, const char* data, size_t size) {
    try {
        std::filesystem::path scriptPath(name);
        std::string normalizedPath = scriptPath.filename().string();
        
        if (scriptPath.extension() == ".phs") {
            normalizedPath = scriptPath.stem().string();
        }
        
        m_scripts[normalizedPath] = std::string(data, size);
        return true;
    }
    catch (const std::exception& e) {
        Debug::Error("Failed to load script " + name + ": " + e.what());
        return false;
    }
}

const std::string& ScriptHandler::GetScript(const std::string& name) const {
    static const std::string empty;
    
    auto it = m_scripts.find(name);
    if (it != m_scripts.end()) {
        return it->second;
    }
    
    std::filesystem::path scriptPath(name);
    std::string baseName = scriptPath.filename().string();
    it = m_scripts.find(baseName);
    if (it != m_scripts.end()) {
        return it->second;
    }
    
    baseName = scriptPath.stem().string();
    it = m_scripts.find(baseName);
    if (it != m_scripts.end()) {
        return it->second;
    }
    
    Debug::Error("Script not found: " + name);
    return empty;
} 