#include "AssetManager.h"
#include "Debug.h"
#include <filesystem>
#include <algorithm>

bool AssetManager::LoadAsset(const std::string& path, const char* data, size_t size) {
    try {
        std::filesystem::path assetPath(path);
        std::string normalizedPath = assetPath.string();
        std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
        
        m_assets[normalizedPath] = std::vector<char>(data, data + size);
        return true;
    }
    catch (const std::exception& e) {
        Debug::Error("Failed to load asset " + path + ": " + e.what());
        return false;
    }
}

const std::vector<char>& AssetManager::GetAsset(const std::string& path) const {
    auto it = m_assets.find(path);
    if (it != m_assets.end()) {
        return it->second;
    }
    
    std::filesystem::path assetPath(path);
    std::string normalizedPath = assetPath.string();
    std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
    
    it = m_assets.find(normalizedPath);
    if (it != m_assets.end()) {
        return it->second;
    }
    
    Debug::Error("Asset not found: " + path);
    return m_emptyAsset;
}

void AssetManager::Clear() {
    m_assets.clear();
} 