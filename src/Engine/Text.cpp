#include "Text.h"
#include <iostream>
#define GL_CLAMP_TO_EDGE 0x812F 

FT_Library Text::ft = nullptr;
bool Text::initialized = false;

Text::Text() : face(nullptr), fontSize(0) {}

Text::~Text() {
    if (face) {
        FT_Done_Face(face);
    }

    for (auto& pair : characters) {
        glDeleteTextures(1, &pair.second.textureID);
    }
}

bool Text::initFreeType() {
    if (initialized) return true;
    
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Failed to initialize FreeType" << std::endl;
        return false;
    }
    
    initialized = true;
    return true;
}

void Text::shutdownFreeType() {
    if (initialized) {
        FT_Done_FreeType(ft);
        initialized = false;
    }
}

bool Text::loadFont(const std::string& fontPath, unsigned int size) {
    if (!initialized) {
        if (!initFreeType()) return false;
    }

    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "Failed to load font: " << fontPath << std::endl;
        return false;
    }

    fontSize = size;
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load glyph: " << c << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters.insert(std::pair<char, Character>(c, character));
    }

    return true;
}

void Text::setColor(float red, float green, float blue, float alpha) {
    r = red;
    g = green;
    b = blue;
    a = alpha;
}

void Text::setFormat(const std::string& fontPath, unsigned int size, float r, float g, float b, TextAlign align) {
    loadFont(fontPath, size);
    this->r = r;
    this->g = g;
    this->b = b;
    this->alignment = align;
}

void Text::setBorderStyle(BorderStyle style, float r, float g, float b, float size) {
    borderStyle = style;
    borderR = r;
    borderG = g;
    borderB = b;
    borderSize = size;
}

void Text::render() {
    if (text.empty()) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float baseline = y;
    float xpos = x;

    float totalWidth = 0;
    for (char c : text) {
        Character ch = characters[c];
        totalWidth += (ch.advance >> 6) * scale;
    }

    if (alignment == TextAlign::CENTER) {
        xpos = x + (width - totalWidth) / 2;
    }
    else if (alignment == TextAlign::RIGHT) {
        xpos = x + width - totalWidth;
    }

    if (borderStyle != BorderStyle::NONE) {
        float originalX = xpos;
        float originalY = baseline;

        glColor4f(borderR, borderG, borderB, borderA);
        
        if (borderStyle == BorderStyle::OUTLINE) {
            const float offsets[8][2] = {
                {-borderSize, -borderSize}, {borderSize, -borderSize},
                {-borderSize, borderSize},  {borderSize, borderSize},
                {-borderSize, 0},          {borderSize, 0},
                {0, -borderSize},          {0, borderSize}
            };

            for (const auto& offset : offsets) {
                xpos = originalX + offset[0];
                baseline = originalY + offset[1];
                renderText(xpos, baseline);
            }
        }
        else if (borderStyle == BorderStyle::SHADOW) {
            xpos = originalX + borderSize;
            baseline = originalY + borderSize;
            renderText(xpos, baseline);
            xpos = originalX;
            baseline = originalY;
        }
    }

    glColor4f(r, g, b, a);
    renderText(xpos, baseline);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void Text::renderText(float xpos, float baseline) {
    for (char c : text) {
        Character ch = characters[c];
        float xpos_char = xpos + ch.bearingX * scale;
        float ypos_char = baseline - ch.bearingY * scale;
        float w = ch.width * scale;
        float h = ch.height * scale;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos_char, ypos_char);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos_char + w, ypos_char);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos_char + w, ypos_char + h);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos_char, ypos_char + h);
        glEnd();

        xpos += (ch.advance >> 6) * scale;
    }
} 