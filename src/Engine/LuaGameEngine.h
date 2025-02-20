#pragma once
#include "LuaManager.h"
#include <string>

class LuaGameEngine {
private:
    LuaManager luaManager;

public:
    LuaGameEngine() = default;
    ~LuaGameEngine() = default;

    bool loadScript(const std::string& scriptName);
    void update(float deltaTime);
    lua_State* getLuaState() { return luaManager.getLuaState(); }
}; 