#pragma once

#include <string>
#include <memory>

class Texture
{
private:
    unsigned m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width;
    int m_Height;
    int m_BPP;
public:
    Texture(const std::string& filePath, bool flip);
    ~Texture();

    void Bind(unsigned slot = 0) const;
    void Unbind() const;

    inline const int GetWidth() const { return m_Width; }
    inline const int GetHeight() const { return m_Height; }
};
