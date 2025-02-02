#pragma once
#include <vector>
#include <memory>
#include <string>

class State {
public:
    virtual ~State() = default;
    virtual void Create() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Destroy() = 0;
};

class StateManager {
public:
    static StateManager& GetInstance();
    void PushState(std::unique_ptr<State> state);
    void PopState();
    void Update();
    void Render();

private:
    StateManager() = default;
    std::vector<std::unique_ptr<State>> m_states;
}; 