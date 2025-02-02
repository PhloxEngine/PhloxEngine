#pragma once
#include <memory>
#include <stack>
#include "State.h"

class StateManager {
public:
    static StateManager& GetInstance() {
        static StateManager instance;
        return instance;
    }

    void Push(std::unique_ptr<State> state);
    void Pop();
    void Switch(std::unique_ptr<State> state);
    void Update(float elapsed);
    void Render();
    
    State* GetCurrentState() { 
        return m_states.empty() ? nullptr : m_states.top().get(); 
    }

private:
    StateManager() = default;
    std::stack<std::unique_ptr<State>> m_states;
}; 