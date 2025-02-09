#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class AssetManager {
public:
    static AssetManager& GetInstance() {
        static AssetManager instance;
        return instance;
    }

    bool LoadAsset(const std::string& path, const char* data, size_t size);
    const std::vector<char>& GetAsset(const std::string& path) const;
    void Clear();
    const std::unordered_map<std::string, std::vector<char>>& GetAssets() const { return m_assets; }

private:
    AssetManager() = default;
    std::unordered_map<std::string, std::vector<char>> m_assets;
    std::vector<char> m_emptyAsset;
}; 