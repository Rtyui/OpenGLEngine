#pragma once

#include <vendor/glm/glm.hpp>
#include <string>

class FontCharacter
{
public:

    unsigned m_Ascii;
    glm::vec2 m_TexCoord;
    glm::vec2 m_TextureSize;
    glm::vec2 m_ScreenSize;
    glm::vec2 m_Offset;
    float m_Advance;

public:

    static FontCharacter* CreateFromFontFileLine(const std::string& line, const glm::ivec2& textureSize, const glm::uvec4 padding);
};

