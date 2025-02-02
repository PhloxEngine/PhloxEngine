 #include "State.h"
#include "Application.h"
#include "ScriptHandler.h"
#include "Debug.h"

void State::LoadScript(const std::string& scriptName) {
    const auto& script = ScriptHandler::GetInstance().GetScript(scriptName);
    if (script.empty()) {
        Debug::Error("Failed to load script for state: " + scriptName);
        return;
    }
    m_script = script;
    Debug::Trace("Loaded script: " + scriptName);
}

Application& State::GetApp() {
    return Application::GetInstance();
}