#include "ScriptableSprite.h"
#include "Application.h"
#include "ScriptParser.h"
#include <sstream>
#include <iostream>
#include <filesystem>
#include <windows.h>

#undef RegisterClass

std::map<std::string, ScriptableSprite*> ScriptableSprite::s_sprites;
std::string ScriptableSprite::s_nextId = "sprite0";
std::vector<ScriptableSprite*> ScriptableSprite::s_activeSprites;

ScriptableSprite::ScriptableSprite(SDL_Renderer* renderer) 
    : Sprite(renderer) {
    m_id = s_nextId;
    s_sprites[m_id] = this;
    
    int id = std::stoi(s_nextId.substr(6));
    s_nextId = "sprite" + std::to_string(id + 1);
}

ScriptableSprite::ScriptableSprite(SDL_Renderer* renderer, const std::string& path)
    : Sprite(renderer, path) {
    m_id = s_nextId;
    int id = std::stoi(s_nextId.substr(6));
    s_nextId = "sprite" + std::to_string(id + 1);
}

ScriptableSprite::~ScriptableSprite() {
    auto it = std::find(s_activeSprites.begin(), s_activeSprites.end(), this);
    if (it != s_activeSprites.end()) {
        s_activeSprites.erase(it);
    }
    s_sprites.erase(m_id);
}

void ScriptableSprite::RegisterScriptApi() {
    auto& parser = ScriptParser::GetInstance();
    
    parser.RegisterClass("Sprite")
        .AddStaticFunction("create", &ScriptableSprite::Create)
        .AddFunction("setPosition", &ScriptableSprite::SetPosition)
        .AddFunction("setScale", &ScriptableSprite::SetScale)
        .AddFunction("render", &ScriptableSprite::Render)
        .AddFunction("setVisible", &ScriptableSprite::SetVisible)
        .AddFunction("destroy", &ScriptableSprite::Destroy)
        .AddFunction("add", &ScriptableSprite::Add);
}

void ScriptableSprite::Create(const std::string& args) {
    std::string path = args.substr(1, args.length() - 2);
   // std::cout << "Creating sprite with path: " << path << std::endl;
    char cwd[260];
    GetCurrentDirectoryA(260, cwd);
    //std::cout << "Current working directory: " << cwd << std::endl;
    
    auto* sprite = new ScriptableSprite(Application::GetInstance().GetRenderer(), path);
    sprite->setVisible(true);
    
    std::string currentId = s_nextId;
    s_sprites[currentId] = sprite;
    
    int id = std::stoi(s_nextId.substr(6));
    s_nextId = "sprite" + std::to_string(id + 1);
    
   // std::cout << "Sprite created with ID: " << currentId << std::endl;
   // std::cout << "Number of sprites in map: " << s_sprites.size() << std::endl;
}

void ScriptableSprite::SetPosition(const std::string& args) {
    if (s_sprites.empty()) {
        std::cerr << "No sprites available for SetPosition" << std::endl;
        return;
    }
    auto* sprite = s_sprites.rbegin()->second;
    std::stringstream ss(args);
    float x, y;
    ss >> x >> y;
    std::cout << "Setting sprite position to " << x << ", " << y << std::endl;
    sprite->setPosition(x, y);
}

void ScriptableSprite::SetScale(const std::string& args) {
    if (s_sprites.empty()) {
        //std::cerr << "No sprites available for SetScale" << std::endl;
        return;
    }
    auto* sprite = s_sprites.rbegin()->second;
    std::stringstream ss(args);
    float x, y;
    ss >> x >> y;
    //std::cout << "Setting sprite scale to " << x << ", " << y << std::endl;
    sprite->setScale(x, y);
}

void ScriptableSprite::SetVisible(const std::string& args) {
    if (s_sprites.empty()) {
        std::cerr << "No sprites available for SetVisible" << std::endl;
        return;
    }
    auto* sprite = s_sprites.rbegin()->second;
    bool visible = (args == "true");
    //std::cout << "Setting sprite visibility to " << visible << std::endl;
    sprite->setVisible(visible);
}

void ScriptableSprite::Render(const std::string& args) {
    if (s_sprites.empty()) {
        std::cerr << "No sprites available for Render" << std::endl;
        return;
    }
    auto* sprite = s_sprites.rbegin()->second;
    //std::cout << "Rendering sprite " << sprite->m_id << std::endl;
    if (!sprite->isVisible()) {
        std::cout << "Sprite " << sprite->m_id << " is not visible" << std::endl;
        return;
    }
    if (!sprite->getWidth() || !sprite->getHeight()) {
        std::cout << "Sprite " << sprite->m_id << " has invalid dimensions: " 
                 << sprite->getWidth() << "x" << sprite->getHeight() << std::endl;
        return;
    }
    sprite->render();
}

void ScriptableSprite::Destroy(const std::string& args) {
    if (s_sprites.empty()) return;
    auto* sprite = s_sprites.begin()->second;
    delete sprite;
}

void ScriptableSprite::Add(const std::string& args) {
    if (s_sprites.empty()) {
        std::cerr << "No sprites available to add" << std::endl;
        return;
    }
    auto* sprite = s_sprites.rbegin()->second;
    if (std::find(s_activeSprites.begin(), s_activeSprites.end(), sprite) == s_activeSprites.end()) {
        s_activeSprites.push_back(sprite);
        std::cout << "Added sprite " << sprite->m_id << " to active sprites list" << std::endl;
    }
}

void ScriptableSprite::RenderAll() {
    for (auto* sprite : s_activeSprites) {
        if (sprite && sprite->isVisible()) {
            sprite->render();
        }
    }
} 