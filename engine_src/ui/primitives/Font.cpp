#include "ui/primitives/Font.h"
#include "interface/Debug.h"
#include "interface/Display.h"

#include <fstream>
#include <sstream>

Font::Font() :
    m_FontCharacters{nullptr}, m_SmallestCharSize(1000.f)
{

}

Font::~Font()
{
    for (FontCharacter* fontCharacter : m_FontCharacters)
    {
        if (fontCharacter)
        {
            delete fontCharacter;
        }
    }

    delete m_TextureAtlas;
}


Font* Font::CreateFontFromFile(const std::string& fntPath, const std::string& texPath)
{
    std::ifstream file(fntPath);
    if (!file.is_open())
    {
        Debug::Log(Debug::Warning, "Could not open file " + fntPath);
        return nullptr;
    }
    Font* font = new Font();
    std::string line;
    std::string word;
    glm::uvec4 padding(0);

    font->m_TextureAtlas = new Texture(texPath, false);

    while (std::getline(file, line))
    {
        if (line.rfind("info ", 0) == 0)
        {
            std::stringstream infoSS(line);
            while (infoSS >> word)
            {
                if (word.rfind("size", 0) == 0)
                {
                    sscanf_s(word.c_str(), "size=%u", &font->m_BaseFontSize);
                }
                if (word.rfind("padding", 0) == 0)
                {
                    sscanf_s(word.c_str(), "padding=%u,%u,%u,%u", &padding[0], &padding[1], &padding[2], &padding[3]);
                }
            }
        }

        if (line.rfind("common ", 0) == 0)
        {
            std::stringstream commonSS(line);
            while (commonSS >> word)
            {
                if (word.rfind("lineHeight", 0) == 0)
                {
                    unsigned lineHeight = 0;
                    sscanf_s(word.c_str(), "lineHeight=%u", &lineHeight);
                    font->m_LineHeight = (float)lineHeight - padding.x - padding.z;
                }

                if (word.rfind("base", 0) == 0)
                {
                    unsigned base = 0;
                    sscanf_s(word.c_str(), "base=%u", &font->m_Base);
                }
            }
        }

        if (line.rfind("char ", 0) == 0)
        {
            FontCharacter* fontCharacter = FontCharacter::CreateFromFontFileLine(line,
                glm::ivec2(font->m_TextureAtlas->GetWidth(), font->m_TextureAtlas->GetHeight()), padding);
            if (fontCharacter)
            {
                font->m_FontCharacters[fontCharacter->m_Ascii] = fontCharacter;
                if (fontCharacter->m_Ascii == ' ')
                {
                    font->m_SpaceAdvance = fontCharacter->m_Advance;
                }
                if ((fontCharacter->m_Ascii >= 32) && (fontCharacter->m_Advance < font->m_SmallestCharSize))
                {
                    font->m_SmallestCharSize = fontCharacter->m_Advance;
                }
            }
        }
    }


    return font;
}
