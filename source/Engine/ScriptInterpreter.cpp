#include "ScriptInterpreter.h"
#include "Debug.h"
#include <regex>
#include <sstream>

class ScriptedState : public State {
public:
    void Create() override {
        if (!m_createCode.empty()) {
            ExecuteCode(m_createCode);
        }
    }
    
    void Update(float elapsed) override {
        if (!m_updateCode.empty()) {
            ExecuteCode(m_updateCode);
        }
    }
    
    void Render() override {
        if (!m_renderCode.empty()) {
            ExecuteCode(m_renderCode);
        }
    }
    
    void Destroy() override {
        if (!m_destroyCode.empty()) {
            ExecuteCode(m_destroyCode);
        }
    }
    
    void SetFunctionCode(const std::string& function, const std::string& code) {
        if (function == "Create") m_createCode = code;
        else if (function == "Update") m_updateCode = code;
        else if (function == "Render") m_renderCode = code;
        else if (function == "Destroy") m_destroyCode = code;
    }

private:
    void ExecuteCode(const std::string& code) {
        std::istringstream stream(code);
        std::string line;
        
        const char* tracePattern = "trace\\(\"([^\"]+)\"\\)";
        std::regex traceRegex(tracePattern);
        
        while (std::getline(stream, line)) {
            std::smatch match;
            if (std::regex_search(line, match, traceRegex)) {
                trace(match[1].str());
            }
        }
    }
    
    std::string m_createCode;
    std::string m_updateCode;
    std::string m_renderCode;
    std::string m_destroyCode;
};

void ScriptInterpreter::ParseStateClass(const std::string& script, ScriptedState* state) {
    std::istringstream stream(script);
    std::string line;
    std::string currentFunction;
    std::string functionCode;
    
    const char* classPattern = "class\\s+(\\w+)\\s*:\\s*public\\s+State";
    const char* functionPattern = "void\\s+(\\w+)\\s*\\(\\)\\s*override\\s*\\{";
    const char* closeBracePattern = "\\s*\\}\\s*";
    
    std::regex classRegex(classPattern);
    std::regex functionRegex(functionPattern);
    std::regex closeBraceRegex(closeBracePattern);
    
    bool inFunction = false;
    
    while (std::getline(stream, line)) {
        if (std::regex_search(line, classRegex)) {
            Debug::Trace("Found state class definition");
            continue;
        }
        
        std::smatch match;
        if (std::regex_search(line, match, functionRegex)) {
            if (inFunction) {
                state->SetFunctionCode(currentFunction, functionCode);
            }
            currentFunction = match[1].str();
            functionCode.clear();
            inFunction = true;
            continue;
        }
        
        if (inFunction) {
            if (std::regex_match(line, closeBraceRegex)) {
                state->SetFunctionCode(currentFunction, functionCode);
                inFunction = false;
                continue;
            }
            functionCode += line + "\n";
        }
    }
    
    if (inFunction) {
        state->SetFunctionCode(currentFunction, functionCode);
    }
}

std::unique_ptr<State> ScriptInterpreter::CreateStateFromScript(const std::string& script) {
    auto state = std::make_unique<ScriptedState>();
    ParseStateClass(script, static_cast<ScriptedState*>(state.get()));
    return state;
} 