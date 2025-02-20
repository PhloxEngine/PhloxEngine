#pragma once
#include <string>
#include <filesystem>
#include "AnimatedSprite.h"
extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

class LuaManager {
private:
    lua_State* L;
    std::filesystem::path scriptPath;
    void registerSpriteMethods();
    void registerEngineFunctions();

public:
    LuaManager();
    ~LuaManager();
    bool loadScript(const std::string& scriptName);
    void update(float deltaTime);
    lua_State* getLuaState() { return L; }
}; 