#pragma once
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glut.h>
#include <map>

class Text {
public:
    enum class TextAlign {
        LEFT,
        CENTER,
        RIGHT
    };

    enum class BorderStyle {
        NONE,
        OUTLINE,
        SHADOW
    };

private:
    struct Character {
        unsigned int textureID;
        int width;
        int height;
        int bearingX;
        int bearingY;
        unsigned int advance;
    };

    static FT_Library ft;
    static bool initialized;
    FT_Face face;
    std::map<char, Character> characters;
    float x = 0, y = 0;
    float scale = 1.0f;
    std::string text;
    unsigned int fontSize;
    float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
    TextAlign alignment = TextAlign::LEFT;
    BorderStyle borderStyle = BorderStyle::NONE;
    float borderSize = 0.0f;
    float borderR = 0.0f, borderG = 0.0f, borderB = 0.0f, borderA = 1.0f;
    float width = 0.0f;

    void renderText(float xpos, float baseline);

public:
    Text();
    ~Text();

    bool loadFont(const std::string& fontPath, unsigned int fontSize);
    void setPosition(float x, float y) { this->x = x; this->y = y; }
    void setText(const std::string& text) { this->text = text; }
    void setScale(float scale) { this->scale = scale; }
    void setColor(float red, float green, float blue, float alpha = 1.0f);
    void render();
    void setFormat(const std::string& fontPath, unsigned int size, float r, float g, float b, TextAlign align = TextAlign::LEFT);
    void setBorderStyle(BorderStyle style, float r, float g, float b, float size = 1.0f);
    void setWidth(float w) { width = w; }

    static bool initFreeType();
    static void shutdownFreeType();
}; 