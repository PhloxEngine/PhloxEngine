#include "LuaGameEngine.h"

bool LuaGameEngine::loadScript(const std::string& scriptName) {
    return luaManager.loadScript(scriptName);
}

void LuaGameEngine::update(float deltaTime) {
    luaManager.update(deltaTime);
} 