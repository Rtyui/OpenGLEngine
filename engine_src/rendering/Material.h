#pragma once
#include "rendering/internal/Shader.h"
#include "rendering/internal/Texture.h"

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <vendor/pugixml/pugixml.hpp>

class Resources;

class Material
{
private:
    enum class UniformElemetsTypes
    {
        Float,
        Int
    };
    struct MaterialUniform
    {
        std::string name;
        UniformElemetsTypes type;
        unsigned count;
        void* data;
    };
private:
    std::string m_Name;
    Shader* m_Shader;
    std::unordered_map<std::string, Texture*> m_Textures;
    std::vector<MaterialUniform> m_Uniforms;
public:
    Material(Shader* shader);
    Material();
    ~Material();
    void AddTexture(const std::string& name, Texture* texture);
    void Bind();

    inline Shader* GetShader() const { return m_Shader; }
    inline const std::string& GetName() const { return m_Name; }
    static Material* LoadXMLMaterial(const std::string& name, const std::string& filePath);
private:
    template <typename T> void LoadNumericUniformDataFromXMLNode(MaterialUniform& materialUniform, const pugi::xml_node& node);
};
