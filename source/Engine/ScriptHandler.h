#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class ScriptHandler {
public:
    static ScriptHandler& GetInstance() {
        static ScriptHandler instance;
        return instance;
    }

    bool LoadScript(const std::string& name, const char* data, size_t size);
    const std::string& GetScript(const std::string& name) const;

private:
    ScriptHandler() = default;
    std::unordered_map<std::string, std::string> m_scripts;
}; 