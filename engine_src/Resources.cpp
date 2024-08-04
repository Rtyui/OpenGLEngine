#include "Resources.h"
#include "interface/Debug.h"
#include "rendering/internal/UniformBlocksManager.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
namespace fs = std::filesystem;

Resources* Resources::s_Resources = nullptr;
std::string Resources::s_ResPath = "res/";

Resources::Resources()
{
    if (s_Resources)
    {
        Debug::Log(Debug::Error, "Recreation of resources class singleton!");
        exit(0);
    }

    s_Resources = this;
    UniformBlocksManager::CreateUniformBlocksData();
    LoadShadersDirectory(s_ResPath + "shaders");
    UniformBlocksManager::BindBuffersRange(); // needed to be called after loading all shaders
    LoadTexturesDirectory(s_ResPath + "textures");
    LoadFontsDirectory(s_ResPath + "fonts");
    LoadMaterialsDirectory(s_ResPath + "materials");
    LoadOBJDirectory(s_ResPath + "objs");
    LoadScenesDirectory(s_ResPath + "scenes");

    InitializeUIElements();
}

Resources::~Resources()
{
    for (auto& shaderIt : m_Shaders)
    {
        delete shaderIt.second;
    }

    for (auto& textureIt : m_Textures)
    {
        delete textureIt.second;
    }

    for (auto& meshIt : m_Meshes)
    {
        delete meshIt.second;
    }

    for (auto& materialIt : m_Materials)
    {
        delete materialIt.second;
    }

    for (auto& sceneIt : m_Scenes)
    {
        delete sceneIt.second;
    }

    for (auto& fontIt : m_Fonts)
    {
        delete fontIt.second;
    }
}

Resources* Resources::Singleton()
{
    if (!s_Resources)
    {
        s_Resources = new Resources();
    }
    return s_Resources;
}

void Resources::Destroy()
{
    if (!s_Resources)
    {
        return;
    }
    delete s_Resources;
}

Shader* Resources::GetShader(const std::string& name) const
{
    if (m_Shaders.find(name) == m_Shaders.end())
    {
        return nullptr;
    }
    return m_Shaders.at(name);
}

Texture* Resources::GetTexture(const std::string& name) const
{
    if (m_Textures.find(name) == m_Textures.end())
    {
        return m_Textures.at("missing");
    }
    return m_Textures.at(name);
}

Mesh* Resources::GetMesh(const std::string& name) const
{
    if (m_Meshes.find(name) == m_Meshes.end())
    {
        return nullptr;
    }
    return m_Meshes.at(name);
}

Material* Resources::GetMaterial(const std::string& name) const
{
    if (m_Materials.find(name) == m_Materials.end())
    {
        return nullptr;
    }
    return m_Materials.at(name);
}

Scene* Resources::GetScene(const std::string& name) const
{
    if (m_Scenes.find(name) == m_Scenes.end())
    {
        return nullptr;
    }
    return m_Scenes.at(name);
}

Font* Resources::GetFont(const std::string& name) const
{
    if (m_Fonts.find(name) == m_Fonts.end())
    {
        return m_Fonts.at("dialog");
    }
    return m_Fonts.at(name);
}

void Resources::LoadShadersDirectory(const std::string& directoryPath)
{
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_directory())
        {
            LoadShadersDirectory(entry.path().string());
            continue;
        }
        Debug::Log(Debug::Info, "Loading shader " + entry.path().stem().string());
        m_Shaders[entry.path().stem().string()] = new Shader(entry.path().stem().string(), entry.path().string());
    }
}

void Resources::LoadMaterialsDirectory(const std::string& directoryPath)
{
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_directory())
        {
            LoadMaterialsDirectory(entry.path().string());
            continue;
        }
        if (entry.path().extension() != ".xml")
        {
            Debug::Log(Debug::Warning, "Ignoring non xml file " + entry.path().string());
            continue;
        }
        Debug::Log(Debug::Info, "Loading material " + entry.path().stem().string());
        Material* material = Material::LoadXMLMaterial(entry.path().stem().string(), entry.path().string());
        if (material)
        {
            m_Materials[entry.path().stem().string()] = material;
        }
    }
}

void Resources::LoadTexturesDirectory(const std::string& directoryPath)
{
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_directory())
        {
            LoadTexturesDirectory(entry.path().string());
            continue;
        }
        Debug::Log(Debug::Info, "Loading texture " + entry.path().stem().string());
        m_Textures[entry.path().stem().string()] = new Texture(entry.path().string(), true);
    }
}

void Resources::LoadOBJDirectory(const std::string& directoryPath)
{
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_directory())
        {
            LoadOBJDirectory(entry.path().string());
            continue;
        }
        if (entry.path().extension() != ".obj")
        {
            Debug::Log(Debug::Warning, "Ignoring non obj file " + entry.path().string());
            continue;
        }
        Debug::Log(Debug::Info, "Loading obj " + entry.path().stem().string());
        LoadOBJFile(entry.path());
    }
}

void Resources::LoadScenesDirectory(const std::string& directoryPath)
{
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_directory())
        {
            LoadScenesDirectory(entry.path().string());
            continue;
        }
        if (entry.path().extension() != ".xml")
        {
            Debug::Log(Debug::Warning, "Ignoring non xml file " + entry.path().string());
            continue;
        }
        Debug::Log(Debug::Info, "Loading scene " + entry.path().stem().string());
        Scene* scene = Scene::CreateFromXMLFile(entry.path().stem().string(), entry.path().string());
        if (scene)
        {
            m_Scenes[entry.path().stem().string()] = scene;
        }
    }
}

void Resources::LoadFontsDirectory(const std::string& directoryPath)
{
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_directory())
        {
            LoadFontsDirectory(entry.path().string());
            continue;
        }

        if (entry.path().extension() == ".png")
        {
            continue;
        }

        if (entry.path().extension() != ".fnt")
        {
            Debug::Log(Debug::Warning, "Ignoring non fnt file " + entry.path().string());
            continue;
        }

        Debug::Log(Debug::Info, "Loading font " + entry.path().stem().string());
        Font* font = Font::CreateFontFromFile(entry.path().parent_path().string() + "/" + entry.path().stem().string() + ".fnt",
            entry.path().parent_path().string() + "/" + entry.path().stem().string() + ".png");
        if (font)
        {
            m_Fonts[entry.path().stem().string()] = font;
        }
    }
}


void Resources::LoadOBJFile(const std::filesystem::path& path)
{
    std::ifstream file(path.string());
    if (!file.is_open())
    {
        Debug::Log(Debug::Warning, "Could not open file " + path.string());
        return;
    }

    
    std::vector<glm::vec3> positionsData;
    std::vector<glm::vec2> texCoordsData;
    std::vector<glm::vec3> normalsData;

    std::string line;
    std::string type;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);

        iss >> type;
        if (!type.compare("v"))
        {
            glm::vec3 vertex = glm::vec3();
            iss >> vertex.x >> vertex.y >> vertex.z;
            positionsData.push_back(vertex);
        }
        else if (!type.compare("vt"))
        {
            glm::vec2 texCoord = glm::vec2();
            iss >> texCoord.x >> texCoord.y;
            texCoordsData.push_back(texCoord);
        }
        else if (!type.compare("vn"))
        {
            glm::vec3 normal = glm::vec3();
            iss >> normal.x >> normal.y >> normal.z;
            normalsData.push_back(normal);
        }
        else if (!type.compare("f"))
        {
            break;
        }
    }

    std::vector<float> vertices;
    std::vector<unsigned> indices;

    std::string vertex1, vertex2, vertex3;
    unsigned index = 0;
    while (true)
    {
        std::istringstream iss(line);
        iss >> type;
        if (type.compare("f"))
        {
            if (!std::getline(file, line))
            {
                break;
            }
            continue;
        }

        iss >> vertex1 >> vertex2 >> vertex3;

        ProcessFaceVertex(vertex1, vertices, indices, positionsData, texCoordsData, normalsData, index);
        ProcessFaceVertex(vertex2, vertices, indices, positionsData, texCoordsData, normalsData, index);
        ProcessFaceVertex(vertex3, vertices, indices, positionsData, texCoordsData, normalsData, index);

        if (!std::getline(file, line))
        {
            break;
        }
    }
    file.close();

    VertexBufferLayout* layout = new VertexBufferLayout();
    layout->Push<float>("position", 3);
    layout->Push<float>("texCoord", 2);
    layout->Push<float>("normal", 3);

    m_Meshes[path.stem().string()] = Mesh::CreateMesh(vertices, layout, indices, path.stem().string());

}

void Resources::ProcessFaceVertex(const std::string& vertex, std::vector<float>& vertices, std::vector<unsigned>& indices,
    const std::vector<glm::vec3>& positionsData, const std::vector<glm::vec2>& texCoordsData,
    const std::vector<glm::vec3>& normalsData, unsigned& currentIndex)
{
    std::istringstream iss(vertex);
    std::string s;

    std::getline(iss, s, '/');
    unsigned positionIndex = std::stoul(s) - 1;
    std::getline(iss, s, '/');
    unsigned textureIndex = std::stoul(s) - 1;
    std::getline(iss, s, '/');
    unsigned normalIndex = std::stoul(s) - 1;

    indices.push_back(currentIndex++);
    vertices.push_back(positionsData[positionIndex].x);
    vertices.push_back(positionsData[positionIndex].y);
    vertices.push_back(positionsData[positionIndex].z);
 
    vertices.push_back(texCoordsData[textureIndex].x);
    vertices.push_back(texCoordsData[textureIndex].y);
    
    vertices.push_back(normalsData[normalIndex].x);
    vertices.push_back(normalsData[normalIndex].y);
    vertices.push_back(normalsData[normalIndex].z);
}

void Resources::InitializeUIElements()
{
    std::vector<float> quadVertices = {
         0.f,  0.f, 0.f, 1.f,
         0.f,  1.f, 0.f, 0.f,
         1.f,  0.f, 1.f, 1.f,
         1.f,  1.f, 1.f, 0.f
    };


    std::vector<unsigned> quadIndices = {
        0, 1, 2, 1, 3, 2
    };

    VertexBufferLayout* layout = new VertexBufferLayout();
    layout->Push<float>("position", 2);
    layout->Push<float>("texCoord", 2);
    m_Meshes["Quad"] = Mesh::CreateMesh(quadVertices, layout, quadIndices, "Quad");
}
