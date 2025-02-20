#include "LuaManager.h"
#include <iostream>

LuaManager::LuaManager() : scriptPath("Assets/Scripts") {
    L = luaL_newstate();
    luaL_openlibs(L);
    registerEngineFunctions();
}

LuaManager::~LuaManager() {
    if (L) lua_close(L);
}

void LuaManager::registerSpriteMethods() {
    luaL_newmetatable(L, "AnimatedSprite");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    
    lua_pushcfunction(L, [](lua_State* L) -> int {
        auto* sprite = new AnimatedSprite();
        auto** udata = static_cast<AnimatedSprite**>(lua_newuserdata(L, sizeof(AnimatedSprite*)));
        *udata = sprite;
        luaL_getmetatable(L, "AnimatedSprite");
        lua_setmetatable(L, -2);
        return 1;
    });
    lua_setfield(L, -2, "new");

    const luaL_Reg methods[] = {
        {"loadFrames", [](lua_State* L) -> int {
            auto* sprite = *static_cast<AnimatedSprite**>(luaL_checkudata(L, 1, "AnimatedSprite"));
            const char* imagePath = luaL_checkstring(L, 2);
            const char* xmlPath = luaL_checkstring(L, 3);
            sprite->loadFrames(imagePath, xmlPath);
            return 0;
        }},
        {"setPosition", [](lua_State* L) -> int {
            auto* sprite = *static_cast<AnimatedSprite**>(luaL_checkudata(L, 1, "AnimatedSprite"));
            float x = luaL_checknumber(L, 2);
            float y = luaL_checknumber(L, 3);
            sprite->setPosition(x, y);
            return 0;
        }},
        {"addAnimation", [](lua_State* L) -> int {
            auto* sprite = *static_cast<AnimatedSprite**>(luaL_checkudata(L, 1, "AnimatedSprite"));
            const char* name = luaL_checkstring(L, 2);
            const char* prefix = luaL_checkstring(L, 3);
            int fps = luaL_checkinteger(L, 4);
            bool loop = lua_toboolean(L, 5);
            sprite->addAnimation(name, prefix, fps, loop);
            return 0;
        }},
        {"playAnimation", [](lua_State* L) -> int {
            auto* sprite = *static_cast<AnimatedSprite**>(luaL_checkudata(L, 1, "AnimatedSprite"));
            const char* name = luaL_checkstring(L, 2);
            sprite->playAnimation(name);
            return 0;
        }},
        {"update", [](lua_State* L) -> int {
            auto* sprite = *static_cast<AnimatedSprite**>(luaL_checkudata(L, 1, "AnimatedSprite"));
            sprite->update(luaL_optnumber(L, 2, 0.0f));
            return 0;
        }},
        {"render", [](lua_State* L) -> int {
            auto* sprite = *static_cast<AnimatedSprite**>(luaL_checkudata(L, 1, "AnimatedSprite"));
            sprite->render();
            return 0;
        }},
        {nullptr, nullptr}
    };

    luaL_setfuncs(L, methods, 0);
    lua_pop(L, 1);
}

void LuaManager::registerEngineFunctions() {
    lua_pushcfunction(L, [](lua_State* L) -> int {
        std::cout << "Hello from C++!" << std::endl;
        return 0;
    });
    lua_setglobal(L, "printFromEngine");

    registerSpriteMethods();
    
    luaL_newmetatable(L, "Sprite");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    
    lua_pushcfunction(L, [](lua_State* L) -> int {
        auto* sprite = new Sprite();
        *static_cast<Sprite**>(lua_newuserdata(L, sizeof(Sprite*))) = sprite;
        luaL_getmetatable(L, "Sprite");
        lua_setmetatable(L, -2);
        return 1;
    });
    lua_setglobal(L, "Sprite");

    lua_getglobal(L, "AnimatedSprite");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        lua_newtable(L);
        luaL_getmetatable(L, "AnimatedSprite");
        lua_setmetatable(L, -2);
        lua_setglobal(L, "AnimatedSprite");
    }
}

bool LuaManager::loadScript(const std::string& scriptName) {
    std::filesystem::path fullPath = scriptPath / scriptName;
    if (luaL_dofile(L, fullPath.string().c_str()) != LUA_OK) {
        std::cerr << "Error loading script: " << lua_tostring(L, -1) << std::endl;
        return false;
    }
    return true;
}

void LuaManager::update(float deltaTime) {
    lua_getglobal(L, "update");
    if (lua_isfunction(L, -1)) {
        lua_pushnumber(L, deltaTime);
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            std::cerr << "Error in update: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }
    } else {
        lua_pop(L, 1);
    }
} 