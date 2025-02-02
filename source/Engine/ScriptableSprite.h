#pragma once
#include "Sprite.h"
#include "ScriptParser.h"
#include <map>
#include <vector>
#include <algorithm>

class ScriptableSprite : public Sprite {
public:
    ScriptableSprite(SDL_Renderer* renderer);
    ScriptableSprite(SDL_Renderer* renderer, const std::string& path);
    virtual ~ScriptableSprite();

    static void RegisterScriptApi();
    static void RenderAll();

private:
    static std::map<std::string, ScriptableSprite*> s_sprites;
    static std::vector<ScriptableSprite*> s_activeSprites;
    static std::string s_nextId;
    std::string m_id;

    static void Create(const std::string& args);
    static void Destroy(const std::string& args);
    static void Add(const std::string& args);
    void SetPosition(const std::string& args);
    void SetScale(const std::string& args);
    void SetVisible(const std::string& args);
    void Render(const std::string& args);
}; 