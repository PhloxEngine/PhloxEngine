#pragma once
#include <string>
#include "Debug.h"

class State {
public:
    State() = default;
    virtual ~State() = default;

    virtual void Create() {
        if (!m_script.empty()) {
            Debug::Trace(m_script); 
        }
    }
    
    virtual void Update(float elapsed) {}
    virtual void Render() {}
    virtual void Destroy() {}

    void LoadScript(const std::string& scriptName);
    
protected:
    std::string m_script;
    class Application& GetApp();
};