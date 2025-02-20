#pragma once
#include <string>

class Sprite {
protected:
    bool visible = true;
    std::string imagePath;
    float x = 0, y = 0;
    unsigned char* imageData = nullptr;
    unsigned int textureID = 0;
    int width = 0;
    int height = 0;

public:
    struct Scale {
        float x;
        float y;
        Scale() : x(1.0f), y(1.0f) {}
        void set(float newX, float newY) { x = newX; y = newY; }
    };

    Sprite();
    Sprite(const std::string& path);
    virtual ~Sprite(); 

    virtual void update(float deltaTime) {}
    virtual void render(); 

    float getX() const { return x; }
    float getY() const { return y; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }

    Scale scale;

    void setScale(float scaleX, float scaleY);

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    const Scale& getScale() const { return scale; }

    void setVisible(bool visible) { this->visible = visible; }
    bool isVisible() const { return visible; }

protected:
    virtual void loadTexture(const std::string& imagePath);
};