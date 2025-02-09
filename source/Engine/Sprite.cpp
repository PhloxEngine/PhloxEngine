#include "Sprite.h"
#include "Debug.h"
#include "stb_image.h"
#include "AssetManager.h"
#include <filesystem>
#include <algorithm>

Sprite::Sprite() : m_textureId(0), m_width(0), m_height(0) {
    m_color[0] = m_color[1] = m_color[2] = m_color[3] = 1.0f;
}

Sprite::~Sprite() {
    if (m_textureId != 0) {
        glDeleteTextures(1, &m_textureId);
    }
}

bool Sprite::LoadFromFile(const std::string& filename) {
    Debug::Trace("LoadFromFile called with: " + filename);
    
    std::filesystem::path assetPath(filename);
    std::string normalizedPath = assetPath.string();
    std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
    Debug::Trace("Normalized path: " + normalizedPath);
    
    const auto& assetData = AssetManager::GetInstance().GetAsset(normalizedPath);
    if (assetData.empty()) {
        Debug::Trace("Asset data is empty, listing available assets:");
        for (const auto& [path, _] : AssetManager::GetInstance().GetAssets()) {
            Debug::Trace("  - " + path);
        }
        Debug::Error("Failed to load sprite from asset: " + normalizedPath);
        return false;
    }
    
    Debug::Trace("Asset size: " + std::to_string(assetData.size()) + " bytes");
    return LoadFromData(reinterpret_cast<const unsigned char*>(assetData.data()), assetData.size());
}

bool Sprite::LoadFromData(const unsigned char* data, int size) {
    int channels;
    Debug::Trace("Attempting to decode image data...");
    unsigned char* imageData = stbi_load_from_memory(data, size, &m_width, &m_height, &channels, 4);
    
    if (!imageData) {
        Debug::Error("Failed to decode image data: " + std::string(stbi_failure_reason()));
        return false;
    }

    Debug::Trace("Image decoded successfully: " + std::to_string(m_width) + "x" + std::to_string(m_height));
    
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(imageData);
    return true;
}

void Sprite::Render(float x, float y, float scale) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glColor4f(m_color[0], m_color[1], m_color[2], m_color[3]);
    
    float w = m_width * scale;
    float h = m_height * scale;
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x, y);
    glTexCoord2f(1, 0); glVertex2f(x + w, y);
    glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
    glTexCoord2f(0, 1); glVertex2f(x, y + h);
    glEnd();
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void Sprite::SetColor(float r, float g, float b, float a) {
    m_color[0] = r;
    m_color[1] = g;
    m_color[2] = b;
    m_color[3] = a;
} 