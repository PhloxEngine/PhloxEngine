#include "../../include/engine/Sprite.h"
#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../../include/thirdparty/stb_image.h"
#include <iostream>

Sprite::Sprite(const std::string& imagePath) : x(0), y(0), width(0), height(0), imageData(nullptr), textureID(0) {
    loadTexture(imagePath);
}

Sprite::~Sprite() {
    if (imageData) {
        stbi_image_free(imageData);
    }
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

void Sprite::update() {
}

void Sprite::render() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale.x, scale.y, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(width, 0);
    glTexCoord2f(1, 1); glVertex2f(width, height);
    glTexCoord2f(0, 1); glVertex2f(0, height);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void Sprite::loadTexture(const std::string& imagePath) {
    int channels;
    imageData = stbi_load(imagePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!imageData) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
}
