#include "AnimatedSprite.h"
#include <GL/glut.h>
#include <stb_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

AnimatedSprite::AnimatedSprite() : Sprite() {}

AnimatedSprite::AnimatedSprite(const std::string& path) : Sprite(path) {}

AnimatedSprite::~AnimatedSprite() {}

void AnimatedSprite::update(float deltaTime) {
    if (currentAnimation) {
        frameTimer += deltaTime;
        float frameDuration = 1.0f / currentAnimation->frameRate;
        
        while (frameTimer >= frameDuration) {
            currentFrame++;
            if (currentFrame >= currentAnimation->frames.size()) {
                if (currentAnimation->loop) {
                    currentFrame = 0;
                } else {
                    currentFrame = currentAnimation->frames.size() - 1;
                }
            }
            frameTimer -= frameDuration;
        }
    }
}

void AnimatedSprite::render() {
    if (!currentAnimation || currentAnimation->frames.empty()) {
        std::cerr << "No current animation or animation has no frames" << std::endl;
        return;
    }

    const Frame& frame = currentAnimation->frames[currentFrame];

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale.x, scale.y, 1.0f);

    float texLeft = (float)frame.x / width;
    float texRight = (float)(frame.x + frame.width) / width;
    float texTop = (float)frame.y / height;
    float texBottom = (float)(frame.y + frame.height) / height;

    glBegin(GL_QUADS);
    glTexCoord2f(texLeft, texTop); glVertex2f(0, 0);
    glTexCoord2f(texRight, texTop); glVertex2f(frame.width, 0);
    glTexCoord2f(texRight, texBottom); glVertex2f(frame.width, frame.height);
    glTexCoord2f(texLeft, texBottom); glVertex2f(0, frame.height);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void AnimatedSprite::loadTexture(const std::string& imagePath) {
    if (imageData) {
        std::cout << "Texture already loaded, skipping." << std::endl;
        return;
    }

    std::cout << "Attempting to load image from: " << imagePath << std::endl;
    
    int channels;
    imageData = stbi_load(imagePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!imageData) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        std::cerr << "STB Image error: " << stbi_failure_reason() << std::endl;
        return;
    }

    std::cout << "Image loaded successfully. Width: " << width << ", Height: " << height << ", Channels: " << channels << std::endl;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
}

void AnimatedSprite::setScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}

void AnimatedSprite::loadFrames(const std::string& imagePath, const std::string& xmlPath) {
    loadTexture(imagePath);
    parseXML(xmlPath);
}

void AnimatedSprite::parseXML(const std::string& xmlPath) {
    std::cout << "Attempting to parse XML file: " << xmlPath << std::endl;
    std::ifstream file(xmlPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open XML file: " << xmlPath << std::endl;
        return;
    }

    std::string line;
    int frameCount = 0;
    while (std::getline(file, line)) {
        if (line.find("<SubTexture") != std::string::npos) {
            Frame frame;
            size_t nameStart = line.find("name=\"") + 6;
            size_t nameEnd = line.find("\"", nameStart);
            frame.name = line.substr(nameStart, nameEnd - nameStart);

            std::istringstream iss(line);
            std::string token;
            while (std::getline(iss, token, ' ')) {
                size_t pos = token.find('=');
                if (pos != std::string::npos) {
                    std::string key = token.substr(0, pos);
                    std::string value = token.substr(pos + 2, token.length() - pos - 3);
                    if (key == "x") frame.x = std::stoi(value);
                    else if (key == "y") frame.y = std::stoi(value);
                    else if (key == "width") frame.width = std::stoi(value);
                    else if (key == "height") frame.height = std::stoi(value);
                    else if (key == "frameX") frame.frameX = std::stoi(value);
                    else if (key == "frameY") frame.frameY = std::stoi(value);
                    else if (key == "frameWidth") frame.frameWidth = std::stoi(value);
                    else if (key == "frameHeight") frame.frameHeight = std::stoi(value);
                }
            }
            frames[frame.name] = frame;
            frameCount++;
        }
    }
}

void AnimatedSprite::addAnimation(const std::string& name, const std::string& prefix, int fps, bool loop) {
    Animation animation;
    animation.name = name;
    animation.frameRate = fps;
    animation.loop = loop;

    for (const auto& pair : frames) {
        if (pair.first.find(prefix) != std::string::npos) {
            animation.addFrame(pair.second);
        }
    }

    animations[name] = animation;
}

void AnimatedSprite::addAnimation(const std::string& name, const std::string& prefix, 
                                const std::vector<int>& indices, int fps, bool loop) {
    Animation animation;
    animation.name = name;
    animation.frameRate = fps;
    animation.loop = loop;

    for (int index : indices) {
        std::string frameName = prefix + " " + std::to_string(index);
        if (frames.find(frameName) != frames.end()) {
            animation.addFrame(frames[frameName]);
        }
    }

    animations[name] = animation;
}

void AnimatedSprite::addAnimation(const std::string& name, const std::vector<std::string>& frameNames, int fps, bool loop) {
    Animation animation;
    animation.name = name;
    animation.frameRate = fps;
    animation.loop = loop;

    for (const auto& frameName : frameNames) {
        if (frames.find(frameName) != frames.end()) {
            animation.addFrame(frames[frameName]);
        }
    }

    animations[name] = animation;
}

void AnimatedSprite::playAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        currentAnimation = &it->second;
        currentFrame = 0;
        frameTimer = 0;
    } else {
        std::cerr << "Animation not found: " << name << std::endl;
    }
}