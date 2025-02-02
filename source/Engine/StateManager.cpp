#include "StateManager.h"

StateManager& StateManager::GetInstance() {
    static StateManager instance;
    return instance;
}

void StateManager::PushState(std::unique_ptr<State> state) {
    state->Create();
    m_states.push_back(std::move(state));
}

void StateManager::PopState() {
    if (!m_states.empty()) {
        m_states.back()->Destroy();
        m_states.pop_back();
    }
}

void StateManager::Update() {
    if (!m_states.empty()) {
        m_states.back()->Update();
    }
}

void StateManager::Render() {
    if (!m_states.empty()) {
        m_states.back()->Render();
    }
} 