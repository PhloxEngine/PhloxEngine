#include "StateManager.h"

void StateManager::Push(std::unique_ptr<State> state) {
    if (state) {
        state->Create();
        m_states.push(std::move(state));
    }
}

void StateManager::Pop() {
    if (!m_states.empty()) {
        m_states.top()->Destroy();
        m_states.pop();
    }
}

void StateManager::Switch(std::unique_ptr<State> state) {
    while (!m_states.empty()) {
        Pop();
    }
    Push(std::move(state));
}

void StateManager::Update(float elapsed) {
    if (!m_states.empty()) {
        m_states.top()->Update(elapsed);
    }
}

void StateManager::Render() {
    if (!m_states.empty()) {
        m_states.top()->Render();
    }
} 