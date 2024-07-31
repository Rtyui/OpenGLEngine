#pragma once
#include <string>
#include <vendor/glm/glm.hpp>
#include <unordered_map>
#include <memory>

struct ShaderUniformData
{
    std::string name;
    unsigned type;
    int location;
};

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    typedef void (*UniformSetFunc)(int, unsigned, void*);
private:
    std::string m_Name;
    unsigned m_RendererID;
    std::unordered_map<std::string, ShaderUniformData> m_Uniforms;
    std::unordered_map<std::string, unsigned> m_UniformBlocksIndices;

public:
    Shader(const std::string& name, const std::string& filePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform4fv(const std::string& name, glm::vec4 vec);
    void SetUniform3fv(const std::string& name, glm::vec3 vec);
    void SetUniformMat4f(const std::string& name, glm::mat4 matrix);
    template <typename T> void SetUniform(const std::string& name, void* data, unsigned count);
    template <> void SetUniform<float>(const std::string& name, void* data, unsigned count);
    template <> void SetUniform<int>(const std::string& name, void* data, unsigned count);
    bool HasUniform(const std::string& name);

    inline const std::string& GetName() const { return m_Name; }
private:
    void GetUniforms();
    void GetUniformBlocks();
    static ShaderProgramSource ParseShader(const std::string& filePath);
    static unsigned CompileShader(unsigned type, const std::string& source);
    static unsigned CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
