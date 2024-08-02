#include "rendering/Material.h"
#include "interface/Debug.h"
#include "Resources.h"
#include "Utils.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vendor/pugixml/pugixml.hpp>
Material::Material(Shader* shader)
    : m_Shader(shader)
{
}

Material::Material()
    : m_Name("default"), m_Shader(nullptr)
{
}

Material::~Material()
{
    for (MaterialUniform& materialUniform : m_Uniforms)
    {
        delete[] materialUniform.data;
    }
}

void Material::AddTexture(const std::string& name, Texture* texture)
{
    if (m_Textures.find(name) != m_Textures.end())
    {
        Debug::Log(Debug::Warning, "Already added texture " + name);
    }
    m_Textures[name] = texture;
}

void Material::Bind()
{
    m_Shader->Bind();
    unsigned index = 0;
    for (auto it = m_Textures.begin(); it != m_Textures.end(); ++it)
    {
        it->second->Bind(index);
        m_Shader->SetUniform1i(it->first, index); // TODO optimize, set uniform when adding texture
        index++;
    }
    for (MaterialUniform& materialUniform : m_Uniforms)
    {
        if (materialUniform.type == UniformElemetsTypes::Float)
        {
            m_Shader->SetUniform<float>(materialUniform.name, materialUniform.data, materialUniform.count);
        }
        else if (materialUniform.type == UniformElemetsTypes::Int)
        {
            m_Shader->SetUniform<int>(materialUniform.name, materialUniform.data, materialUniform.count);
        }
    }
}

Material* Material::LoadXMLMaterial(const std::string& name, const std::string& filePath)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());
    std::string logPrefix;
    
    if (!result)
    {
        Debug::Log(Debug::Warning, "Could not load xml file " + filePath);
        return nullptr;
    }

    pugi::xml_node mainChild = doc.child("Material");
    if (mainChild.empty())
    {
        Debug::Log(Debug::Warning, "File '" + filePath + "' first child not 'Material'");
        return nullptr;
    }

    std::string materialName(mainChild.attribute("name").value());
    if (materialName != name)
    {
        Debug::Log(Debug::Error, "Name mismatch '" + name + "' != '" + materialName + "'");
        return nullptr;
    }

    Material* material = new Material();
    material->m_Name = name;

    logPrefix += "Material '" + material->m_Name + "' ";
    std::string shaderName(mainChild.attribute("shader").value());
    material->m_Shader = Resources::Singleton()->GetShader(shaderName);
    if (material->m_Shader == nullptr)
    {
        Debug::Log(Debug::Warning, logPrefix + "shader '" + shaderName + "' does not exist!");
        delete material;
        return nullptr;
    }

    for (pugi::xml_node child : mainChild.children())
    {
        std::string childName(child.name());
        if (childName != "Uniform")
        {
            Debug::Log(Debug::Error, "Material '" + filePath + "' child not uniform '" + childName + "'");
            continue;
        }

        MaterialUniform materialUniform;
        materialUniform.name = "u_" + std::string(child.attribute("name").value());
        std::string uniformLogPrefix = logPrefix + "uniform '" + materialUniform.name + "' ";
        std::string uniformType(child.attribute("type").value());
        if (uniformType == "numeric")
        {
            std::string uniformDataType(child.attribute("dataType").value());
            if (uniformDataType == "float")
            {
                materialUniform.type = UniformElemetsTypes::Float;
                material->LoadNumericUniformDataFromXMLNode<float>(materialUniform, child);
            }
            else if (uniformDataType == "int")
            {
                materialUniform.type = UniformElemetsTypes::Int;
                material->LoadNumericUniformDataFromXMLNode<int>(materialUniform, child);
            }
            else
            {
                Debug::Log(Debug::Error, uniformLogPrefix + "unknown data type '" + uniformDataType + "'");
            }
        }
        else if (uniformType == "texture")
        {
            std::string textureName(child.text().as_string());
            Texture* texture = Resources::Singleton()->GetTexture(textureName);
            if (!texture)
            {
                Debug::Log(Debug::Error, uniformLogPrefix + "texture '" + textureName + "' does not exist");
            }
            material->AddTexture(materialUniform.name, texture);
        }
        else
        {
            Debug::Log(Debug::Error, uniformLogPrefix + "unknown uniform type '" + uniformType + "'");
        }
    }

    return material;
}

template <typename T> void Material::LoadNumericUniformDataFromXMLNode(MaterialUniform& materialUniform, const pugi::xml_node& node)
{
    materialUniform.count = std::atoi(node.attribute("count").value());
    std::string display;
    T* data = new T[materialUniform.count];
    char token = 0;
    std::string uniformValue(node.text().as_string());
    std::istringstream iss(uniformValue);
    for (unsigned i = 0; i < materialUniform.count; ++i)
    {
        iss >> data[i] >> token;
    }
    for (unsigned i = 0; i < materialUniform.count; ++i)
    {
        display += std::to_string(data[i]) + (i != materialUniform.count - 1 ? ", " : "");
    }
    Debug::Log(Debug::Verbose, display);
    materialUniform.data = data;
    m_Uniforms.push_back(materialUniform);
}
