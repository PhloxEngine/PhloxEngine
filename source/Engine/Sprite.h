#pragma once
#include <string>
#include <GL/freeglut.h>

class Sprite {
public:
    Sprite();
    ~Sprite();

    bool LoadFromFile(const std::string& filename);
    bool LoadFromMemory(const std::string& assetPath);
    void Render(float x, float y, float scale = 1.0f);
    void SetColor(float r, float g, float b, float a = 1.0f);
    
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    bool LoadFromData(const unsigned char* data, int size);
    GLuint m_textureId;
    int m_width;
    int m_height;
    float m_color[4];
}; 