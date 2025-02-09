#pragma once
#include <string>
#include <memory>
#include "State.h"
#include "Sprite.h"
#include <map>

class ScriptedState; 

class ScriptInterpreter {
public:
    static ScriptInterpreter& GetInstance() {
        static ScriptInterpreter instance;
        return instance;
    }

    std::unique_ptr<State> CreateStateFromScript(const std::string& script);
    std::map<std::string, std::shared_ptr<Sprite>>& GetSprites();
    
private:
    ScriptInterpreter() = default;
    void ParseStateClass(const std::string& script, ScriptedState* state);
    void ExecuteFunction(State* state, const std::string& functionName, const std::string& code);
    std::map<std::string, std::shared_ptr<Sprite>> m_sprites;
}; 