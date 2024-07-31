#pragma once
#include "entities/Scene.h"
#include "ui/primitives/Font.h"
#include "rendering/internal/Shader.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>


class Resources
{
private:
    static Resources* s_Resources;
    static std::string s_ResPath;

    std::unordered_map<std::string, Shader*> m_Shaders;
    std::unordered_map<std::string, Texture*> m_Textures;
    std::unordered_map<std::string, Mesh*> m_Meshes;
    std::unordered_map<std::string, Material*> m_Materials;
    std::unordered_map<std::string, Scene*> m_Scenes;
    std::unordered_map<std::string, Font*> m_Fonts;

public:
    ~Resources();

    static Resources* Singleton();
    static void Destroy();

    Shader* GetShader(const std::string& name) const;
    Texture* GetTexture(const std::string& name) const;
    Mesh* GetMesh(const std::string& name) const;
    Material* GetMaterial(const std::string& name) const;
    Scene* GetScene(const std::string& name) const;
    Font* GetFont(const std::string& name) const;

private:
    Resources();

    void LoadShadersDirectory(const std::string& directoryPath);
    void LoadMaterialsDirectory(const std::string &directoryPath);
    void LoadTexturesDirectory(const std::string& directoryPath);
    void LoadOBJDirectory(const std::string& directoryPath);
    void LoadScenesDirectory(const std::string& directoryPath);
    void LoadFontsDirectory(const std::string& directoryPath);

    void LoadOBJFile(const std::filesystem::path& path);
    void ProcessFaceVertex(const std::string& vertex, std::vector<float>& vertices, std::vector<unsigned>& indices,
        const std::vector<glm::vec3>& positionsData, const std::vector<glm::vec2>& texCoordsData,
        const std::vector<glm::vec3>& normalsData, unsigned& currentIndex);

    void InitializeUIElements();

};

