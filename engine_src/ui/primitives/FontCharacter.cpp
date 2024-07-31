#include "FontCharacter.h"
#include "interface/Debug.h"
#include "interface/Display.h"

#include <sstream>

FontCharacter* FontCharacter::CreateFromFontFileLine(const std::string& line, const glm::ivec2& textureSize, const glm::uvec4 padding)
{
    std::stringstream lineSS(line);
    std::string token;
    FontCharacter* fontCharacter = new FontCharacter();

    lineSS >> token;
    if (token != "char")
    {
        Debug::Log(Debug::Error, "Wrong character format '" + line + "'!");
        delete fontCharacter;
        return nullptr;
    }

    lineSS >> token;
    if (sscanf_s(token.c_str(), "id=%d", &fontCharacter->m_Ascii) != 1)
    {
        Debug::Log(Debug::Error, "Wrong character format '" + line + "'!");
        delete fontCharacter;
        return nullptr;
    }

    lineSS >> token;
    if (sscanf_s(token.c_str(), "x=%f", &fontCharacter->m_TexCoord.x) != 1)
    {
        Debug::Log(Debug::Error, "Wrong character format '" + line + "'!");
        delete fontCharacter;
        return nullptr;
    }
    fontCharacter->m_TexCoord.x = (fontCharacter->m_TexCoord.x + padding.w) / textureSize.x;

    lineSS >> token;
    if (sscanf_s(token.c_str(), "y=%f", &fontCharacter->m_TexCoord.y) != 1)
    {
        Debug::Log(Debug::Error, "Wrong character format '" + line + "'!");
        delete fontCharacter;
        return nullptr;
    }
    fontCharacter->m_TexCoord.y = (fontCharacter->m_TexCoord.y + padding.x) / textureSize.y;

    lineSS >> token;
    if (sscanf_s(token.c_str(), "width=%f", &fontCharacter->m_ScreenSize.x) != 1)
    {
        Debug::Log(Debug::Error, "Wrong character format '" + line + "'!");
        delete fontCharacter;
        return nullptr;
    }
    fontCharacter->m_ScreenSize.x = fontCharacter->m_ScreenSize.x - padding.y - padding.w;
    fontCharacter->m_TextureSize.x = fontCharacter->m_ScreenSize.x / textureSize.x;

    lineSS >> token;
    if (sscanf_s(token.c_str(), "height=%f", &fontCharacter->m_ScreenSize.y) != 1)
    {
        Debug::Log(Debug::Error, "Wrong character format '" + line + "'!");
        delete fontCharacter;
        return nullptr;
    }
    fontCharacter->m_ScreenSize.y = fontCharacter->m_ScreenSize.y - padding.x - padding.z;
    fontCharacter->m_TextureSize.y = fontCharacter->m_ScreenSize.y / textureSize.y;

    lineSS >> token;
    if (sscanf_s(token.c_str(), "xoffset=%f", &fontCharacter->m_Offset.x) != 1)
    {
        Debug::Log(Debug::Error, "Wrong character format '" + line + "'!");
        delete fontCharacter;
        return nullptr;
    }
    fontCharacter->m_Offset.x += padding.w;

    lineSS >> token;
    if (sscanf_s(token.c_str(), "yoffset=%f", &fontCharacter->m_Offset.y) != 1)
    {
        Debug::Log(Debug::Error, "Wrong character format '" + line + "'!");
        delete fontCharacter;
        return nullptr;
    }
    fontCharacter->m_Offset.y += padding.x;

    lineSS >> token;
    if (sscanf_s(token.c_str(), "xadvance=%f", &fontCharacter->m_Advance) != 1)
    {
        Debug::Log(Debug::Error, "Wrong character format '" + line + "'!");
        delete fontCharacter;
        return nullptr;
    }
    fontCharacter->m_Advance = fontCharacter->m_Advance - padding.y - padding.w;
    return fontCharacter;
}
