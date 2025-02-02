#include "Sprite.h"
#include <iostream>
#include <filesystem>
#include <windows.h>

Sprite::Sprite(SDL_Renderer* renderer) : renderer(renderer), imagePath("") {}

Sprite::Sprite(SDL_Renderer* renderer, const std::string& path) 
    : renderer(renderer), imagePath(path) {
    loadTexture(path);
}

Sprite::~Sprite() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Sprite::render() {
    if (!visible) {
        std::cout << "Sprite not visible" << std::endl;
        return;
    }
    if (!texture) {
        std::cout << "Sprite has no texture" << std::endl;
        return;
    }

    SDL_Rect srcRect = { 0, 0, width, height };
    SDL_Rect dstRect = { 
        static_cast<int>(x), 
        static_cast<int>(y), 
        static_cast<int>(width * scale.x), 
        static_cast<int>(height * scale.y) 
    };

    //std::cout << "Rendering sprite at (" << x << "," << y << ") with size " << (width * scale.x) << "x" << (height * scale.y) << std::endl;
              
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

void Sprite::loadTexture(const std::string& imagePath) {
    //std::cout << "Attempting to load texture: " << imagePath << std::endl;
    
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;
        
        char cwd[260];
        GetCurrentDirectoryA(260, cwd);
        std::cerr << "Current working directory: " << cwd << std::endl;
        return;
    }

    width = surface->w;
    height = surface->h;
   // std::cout << "Image loaded successfully. Dimensions: " << width << "x" << height << std::endl;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
    } else {
        std::cout << "Texture created successfully" << std::endl;
    }

    SDL_FreeSurface(surface);
}

void Sprite::setScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}