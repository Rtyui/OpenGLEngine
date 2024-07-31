#pragma once

#include "FontCharacter.h"
#include "rendering/internal/Texture.h"

#include <string>

class Font
{

private:

    FontCharacter* m_FontCharacters[128];
    Texture* m_TextureAtlas;

    unsigned m_BaseFontSize;
    float m_LineHeight;
    float m_SpaceAdvance;
    unsigned m_Base;
    float m_SmallestCharSize;

public:

    ~Font();

    inline FontCharacter* GetCharacter(const char& c) const { return m_FontCharacters[unsigned(c)]; }
    inline const float GetLineHeight() const { return m_LineHeight; }
    inline Texture* GetTextureAtlas() const { return m_TextureAtlas; }
    inline const unsigned& GetBaseFontSize() const { return m_BaseFontSize; }
    inline const float& GetSpaceAdvance() const { return m_SpaceAdvance; }
    inline const unsigned& GetBase() const { return m_Base; }
    inline const float& GetSmallestCharSize() const { return m_SmallestCharSize; }

    static Font* CreateFontFromFile(const std::string& fntPath, const std::string& texPath);

private:

    Font();
};

