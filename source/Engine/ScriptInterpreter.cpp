#include "ScriptInterpreter.h"
#include "Debug.h"
#include <regex>
#include <sstream>

class ScriptedState : public State {
public:
    ScriptedState(ScriptInterpreter& interpreter) : m_interpreter(interpreter) {}
    
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
        const char* spritePattern = "\\s*(\\w+)\\s*=\\s*new\\s+Sprite\\(\\)";
        const char* loadPattern = "(\\w+)->LoadFromFile\\(\"([^\"]+)\"\\)";
        const char* renderPattern = "(\\w+)->Render\\((.*?)\\)";
        const char* clearColorPattern = "glClearColor\\((.*?)\\)";
        const char* clearPattern = "glClear\\((.*?)\\)";
        
        std::regex traceRegex(tracePattern);
        std::regex spriteRegex(spritePattern);
        std::regex loadRegex(loadPattern);
        std::regex renderRegex(renderPattern);
        std::regex clearColorRegex(clearColorPattern);
        std::regex clearRegex(clearPattern);
        
        while (std::getline(stream, line)) {
            std::smatch match;
            if (std::regex_search(line, match, traceRegex)) {
                Debug::Trace(match[1].str());
            }
            else if (std::regex_search(line, match, spriteRegex)) {
                std::string spriteName = match[1].str();
                m_interpreter.GetSprites()[spriteName] = std::make_shared<Sprite>();
                Debug::Trace("Created new sprite: " + spriteName);
            }
            else if (std::regex_search(line, match, loadRegex)) {
                std::string spriteName = match[1].str();
                auto& sprites = m_interpreter.GetSprites();
                if (sprites.find(spriteName) != sprites.end()) {
                    bool success = sprites[spriteName]->LoadFromFile(match[2].str());
                    if (!success) {
                        Debug::Error("Failed to load sprite: " + spriteName);
                    }
                }
            }
            else if (std::regex_search(line, match, renderRegex)) {
                std::string spriteName = match[1].str();
                auto& sprites = m_interpreter.GetSprites();
                if (sprites.find(spriteName) != sprites.end()) {
                    std::string params = match[2].str();
                    float x = 0, y = 0, scale = 1.0f;
                    sscanf_s(params.c_str(), "%f, %f, %f", &x, &y, &scale);
                    sprites[spriteName]->Render(x, y, scale);
                }
            }
            else if (std::regex_search(line, match, clearColorRegex)) {
                float r, g, b, a;
                sscanf_s(match[1].str().c_str(), "%f, %f, %f, %f", &r, &g, &b, &a);
                glClearColor(r, g, b, a);
            }
            else if (std::regex_search(line, match, clearRegex)) {
                glClear(GL_COLOR_BUFFER_BIT);
            }
        }
    }
    
    ScriptInterpreter& m_interpreter;
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

std::map<std::string, std::shared_ptr<Sprite>>& ScriptInterpreter::GetSprites() {
    return m_sprites;
}

std::unique_ptr<State> ScriptInterpreter::CreateStateFromScript(const std::string& script) {
    auto state = std::make_unique<ScriptedState>(*this);
    ParseStateClass(script, static_cast<ScriptedState*>(state.get()));
    return state;
} 