#pragma once
#include <string>
#include <vector>

struct AnimationFrame {
    int x;
    int y;
    int width;
    int height;
    float duration;  // in seconds
};

struct Animation {
    std::string name;
    std::vector<AnimationFrame> frames;
    float frameRate;
    bool looped;
    int currentFrame;
    float frameTimer;
}; 