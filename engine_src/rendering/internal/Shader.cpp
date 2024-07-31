#include "rendering/internal/Shader.h"
#include "interface/Debug.h"
#include "rendering/internal/UniformBlocksManager.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <GL/glew.h>

Shader::Shader(const std::string& name, const std::string& filePath)
    : m_Name(name), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filePath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    GetUniforms();
    GetUniformBlocks();
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(m_Uniforms[name].location, value);
}

void Shader::SetUniform4fv(const std::string& name, glm::vec4 vec)
{
    glUniform4fv(m_Uniforms[name].location, 1, &vec[0]);
}

void Shader::SetUniform3fv(const std::string& name, glm::vec3 vec)
{
    glUniform3fv(m_Uniforms[name].location, 1, &vec[0]);
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4 matrix)
{
    glUniformMatrix4fv(m_Uniforms[name].location, 1, GL_FALSE, &matrix[0][0]);
}

template <typename T> void Shader::SetUniform(const std::string& name, void* data, unsigned count)
{
    Debug::Log(Debug::Error, "Unknown type!");
}

template <> void Shader::SetUniform<float>(const std::string& name, void* data, unsigned count)
{
    auto it = m_Uniforms.find(name);
    if (it == m_Uniforms.end())
    {
        Debug::Log(Debug::Warning, "Shader '" + m_Name + "' does not have uniform '" + name + "'");
        return;
    }

    ShaderUniformData& shaderUniformData = it->second;
    if (count == 1 && shaderUniformData.type == GL_FLOAT)
    {
        glUniform1fv(shaderUniformData.location, 1, (float*)data);
    }
    else if (count == 2 && shaderUniformData.type == GL_FLOAT_VEC2)
    {
        glUniform2fv(shaderUniformData.location, 1, (float*)data);
    }
    else if (count == 3 && shaderUniformData.type == GL_FLOAT_VEC3)
    {
        glUniform3fv(shaderUniformData.location, 1, (float*)data);
    }
    else
    {
        Debug::Log(Debug::Error, "Shader '" + m_Name + "' count mismatch for uniform '" + name + "'");
    }
}

template <> void Shader::SetUniform<int>(const std::string& name, void* data, unsigned count)
{
    auto it = m_Uniforms.find(name);
    if (it == m_Uniforms.end())
    {
        Debug::Log(Debug::Warning, "Shader '" + m_Name + "' does not have uniform '" + name + "'");
        return;
    }

    ShaderUniformData& shaderUniformData = it->second;
    if (count == 1 && shaderUniformData.type == GL_INT)
    {
        glUniform1iv(shaderUniformData.location, 1, (int*)data);
    }
    else if (count == 2 && shaderUniformData.type == GL_INT_VEC2)
    {
        glUniform2iv(shaderUniformData.location, 1, (int*)data);
    }
    else if (count == 3 && shaderUniformData.type == GL_INT_VEC3)
    {
        glUniform3iv(shaderUniformData.location, 1, (int*)data);
    }
    else
    {
        Debug::Log(Debug::Error, "Shader '" + m_Name + "' count mismatch for uniform '" + name + "'");
    }
}

bool Shader::HasUniform(const std::string& name)
{
    return (m_Uniforms.find(name) != m_Uniforms.end());
}

void Shader::GetUniforms()
{
    int count;
    glGetProgramiv(m_RendererID, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &count);
    char nameArr[512];
    unsigned type;
    int size;

    unsigned* indices = new unsigned[count];
    int* offsets = new int[100];

    for (int i = 0; i < count; ++i)
    {
        indices[i] = i;
    }
    glGetActiveUniformsiv(m_RendererID, count, (const unsigned*)indices, GL_UNIFORM_OFFSET, offsets);

    for (int i = 0; i < count; ++i)
    {
        glGetActiveUniform(m_RendererID, i, sizeof(nameArr), nullptr, &size, &type, nameArr);
        // Not doing anything with size yet because i dont have arrays in shader (size is the number of items in array)
        int location = glGetUniformLocation(m_RendererID, nameArr);

        std::string name(nameArr);
        if (offsets[i] != -1)
        {
            Debug::Log(Debug::Verbose, "Skipping " + name + " as it is inside uniform block with offset " + std::to_string(offsets[i]));
            continue;
        }
        ShaderUniformData shaderUniformData;
        shaderUniformData.name = name;
        shaderUniformData.type = type;
        shaderUniformData.location = location;
        m_Uniforms[name] = shaderUniformData;
        Debug::Log(Debug::Verbose, std::string(name) + " length " + std::to_string(name.size()) + " type " + std::to_string(type));
    }

    delete[] indices;
    delete[] offsets;
}

void Shader::GetUniformBlocks()
{
    int numBlocks;
    glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);
    char nameArr[512];
    unsigned blockIndex;

    for (int blockIx = 0; blockIx < numBlocks; ++blockIx)
    {
        GLint nameLen;
        glGetActiveUniformBlockiv(m_RendererID, blockIx, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);

        glGetActiveUniformBlockName(m_RendererID, blockIx, nameLen, NULL, nameArr);
        std::string name(nameArr);
        blockIndex = glGetUniformBlockIndex(m_RendererID, nameArr);
        Debug::Log(Debug::Info, "Adding uniform block " + name + " with index " + std::to_string(blockIndex));
        m_UniformBlocksIndices[name] = blockIndex;
        UniformBlocksManager::BindProgramUniformBlock(m_RendererID, blockIndex, name);
    }
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << std::endl;
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned Shader::CompileShader(unsigned type, const std::string& source)
{
    unsigned id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        delete[] message;
        return 0;
    }

    return id;
}

unsigned Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned program = glCreateProgram();
    unsigned vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
