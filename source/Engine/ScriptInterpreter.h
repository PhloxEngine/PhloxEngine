#pragma once
#include <string>
#include <memory>
#include "State.h"

class ScriptedState; 

class ScriptInterpreter {
public:
    static ScriptInterpreter& GetInstance() {
        static ScriptInterpreter instance;
        return instance;
    }

    std::unique_ptr<State> CreateStateFromScript(const std::string& script);
    
private:
    ScriptInterpreter() = default;
    void ParseStateClass(const std::string& script, ScriptedState* state);
    void ExecuteFunction(State* state, const std::string& functionName, const std::string& code);
}; 