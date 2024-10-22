#include "../../include/engine/State.h"
#include "../../include/engine/SubState.h"
#include <iostream>

void State::openSubState(SubState* subState) {
    std::cout << "State::openSubState called" << std::endl;
    subState->setParentState(this);
    _subStates.push_back(subState);
    subState->create();
}

void State::closeSubState() {
    if (!_subStates.empty()) {
        SubState* subState = _subStates.back();
        if (subState) {
            subState->destroy();
            delete subState;
            _subStates.pop_back();
        }
    }
}

void State::updateSubState(float deltaTime) {
    if (!_subStates.empty()) {
        _subStates.back()->update(deltaTime);
    }
}

void State::renderSubState() {
    if (!_subStates.empty()) {
        _subStates.back()->render();
    }
}
