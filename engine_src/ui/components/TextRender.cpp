#include "ui/components/TextRender.h"
#include "interface/Debug.h"
#include "Resources.h"
#include "components/Transform.h"
#include "interface/TimeClock.h"
#include "interface/Input.h"
#include "utils/XMLUtils.h"

#include <sstream>



TextRender::TextRender() :
    m_Text(""), m_Font(nullptr), m_Mesh(nullptr),m_Color(1.f), m_FontSize(10), m_FontScale(0.f), m_MaxLineSize(0), m_WrapLines(false), m_MaxLinesNumber(0)
{
}

TextRender::TextRender(Font* font, const glm::vec3& color) :
    m_Text(""), m_Font(font), m_Mesh(nullptr), m_Color(color), m_FontSize(10), m_FontScale(0.f), m_MaxLineSize(0), m_WrapLines(false), m_MaxLinesNumber(0)
{
}

TextRender::TextRender(Font* font, const unsigned& fontSize, const glm::vec3& color, const bool& wrapLines, const glm::vec3& size, const std::string& text) :
    m_Text(text), m_Mesh(nullptr), m_Font(font), m_Color(color), m_FontSize(fontSize), m_FontScale(0), m_WrapLines(wrapLines)
{
    Initialize(size);
    FillMesh();
}

TextRender::~TextRender()
{
    delete m_Mesh;
}

void TextRender::Update()
{
}

void TextRender::SetText(const std::string& text)
{
    m_Text = text;
    m_Mesh->Empty();
    FillMesh();
}

void TextRender::SetColor(const glm::vec3& color)
{
    m_Color = color;
}

void TextRender::Initialize(const glm::vec3& size)
{
    unsigned maxCharCount = 0;
    m_MaxLineSize = size.x;

    InitTextParams(size, maxCharCount);

    unsigned verticesSize = maxCharCount * 16;
    unsigned indicesSize = maxCharCount * 6;

    VertexBufferLayout* layout = new VertexBufferLayout();
    layout->Push<float>("position", 2);
    layout->Push<float>("texCoord", 2);

    m_Mesh = Mesh::CreateEmptyMesh(verticesSize, layout, indicesSize, "textMesh");
}

bool TextRender::AddCharacter(std::vector<float>& vertices, std::vector<unsigned>& indices, const char& c, const unsigned& lineIndex, const unsigned& meshIndex, float& cursor)
{
    FontCharacter* fontCharacter = m_Font->GetCharacter(c);
    float lineLevel = lineIndex * m_Font->GetLineHeight() * m_FontScale;

    if (!fontCharacter)
    {
        Debug::Log(Debug::Warning, "Font does not have character " + c);
        return false;
    }

    if ((cursor + fontCharacter->m_Advance * m_FontScale) > m_MaxLineSize)
    {
        Debug::Log(Debug::Warning, "End of line reached!");
        return false;
    }

    if (c == ' ')
    {
        cursor += fontCharacter->m_Advance * m_FontScale;
        return true;
    }

    // Top left position
    vertices.push_back(cursor + fontCharacter->m_Offset.x * m_FontScale);
    vertices.push_back(lineLevel + fontCharacter->m_Offset.y * m_FontScale);

    // Top left tex coord
    vertices.push_back(fontCharacter->m_TexCoord.x);
    vertices.push_back(fontCharacter->m_TexCoord.y);

    // Bottom left position
    vertices.push_back(cursor + fontCharacter->m_Offset.x * m_FontScale);
    vertices.push_back(lineLevel + (fontCharacter->m_Offset.y + fontCharacter->m_ScreenSize.y) * m_FontScale);

    // Bottom left tex coord
    vertices.push_back(fontCharacter->m_TexCoord.x);
    vertices.push_back(fontCharacter->m_TexCoord.y + fontCharacter->m_TextureSize.y);

    // Top right position
    vertices.push_back(cursor + (fontCharacter->m_Offset.x + fontCharacter->m_ScreenSize.x) * m_FontScale);
    vertices.push_back(lineLevel + fontCharacter->m_Offset.y * m_FontScale);

    // Top right tex coord
    vertices.push_back(fontCharacter->m_TexCoord.x + fontCharacter->m_TextureSize.x);
    vertices.push_back(fontCharacter->m_TexCoord.y);

    // Bottom right position
    vertices.push_back(cursor + (fontCharacter->m_Offset.x + fontCharacter->m_ScreenSize.x) * m_FontScale);
    vertices.push_back(lineLevel + (fontCharacter->m_Offset.y + fontCharacter->m_ScreenSize.y) * m_FontScale);

    // Bottom right tex coord
    vertices.push_back(fontCharacter->m_TexCoord.x + fontCharacter->m_TextureSize.x);
    vertices.push_back(fontCharacter->m_TexCoord.y + fontCharacter->m_TextureSize.y);

    indices.push_back(meshIndex);
    indices.push_back(meshIndex + 1);
    indices.push_back(meshIndex + 2);
    indices.push_back(meshIndex + 2);
    indices.push_back(meshIndex + 1);
    indices.push_back(meshIndex + 3);

    cursor += fontCharacter->m_Advance * m_FontScale;
    return true;
}

void TextRender::InitTextParams(const glm::vec3& size, unsigned& maxCharCount)
{
    if (m_WrapLines)
    {
        m_FontScale = (float)m_FontSize / m_Font->GetBaseFontSize();
        m_MaxLinesNumber = (unsigned)(size.y / (m_Font->GetLineHeight() * m_FontScale));
        maxCharCount = (unsigned)(m_MaxLineSize / (m_Font->GetSmallestCharSize() * m_FontScale)) * m_MaxLinesNumber;
        return;
    }
    m_FontScale = size.y / m_Font->GetLineHeight();
    m_MaxLinesNumber = 1;
    maxCharCount = (unsigned)(m_MaxLineSize / (m_Font->GetSmallestCharSize() * m_FontScale));
}

float TextRender::GetTextSize(const std::string& text)
{
    float size = 0.f;
    FontCharacter* fontCharacter;

    for (char c : text)
    {
        fontCharacter = m_Font->GetCharacter(c);
        size += fontCharacter->m_Advance * m_FontScale;
    }

    return size;
}

void TextRender::FillMesh()
{
    std::vector<float> vertices;
    std::vector<unsigned> indices;
    unsigned i = 0;
    unsigned meshIndex = 0;
    float cursor = 0.f;

    if (!m_Text[0])
    {
        return;
    }

    if (m_WrapLines && (m_MaxLinesNumber > 1))
    {
        std::vector<std::string> lines = GetTextLines();
        for (i = 0; i < lines.size() && lines[i][0]; ++i)
        {
            for (char& c : lines[i])
            {
                if (!AddCharacter(vertices, indices, c, i, meshIndex, cursor))
                {
                    Debug::Log(Debug::Error, std::string("Cannot add character '") + c + "'!");
                    return;
                }
                if (c != ' ')
                {
                    meshIndex += 4;
                }
            }
            cursor = 0;
        }
    }
    else
    {
        for (char& c : m_Text)
        {
            if (!AddCharacter(vertices, indices, c, 0, meshIndex, cursor))
            {
                Debug::Log(Debug::Verbose, std::string("Stopping at character '") + c + "'!");
                break;
            }
            if (c != ' ')
            {
                meshIndex += 4;
            }
        }
    }

    m_Mesh->SetData(vertices, indices);
}

std::vector<std::string> TextRender::GetTextLines()
{
    std::vector<std::string> lines;

    std::stringstream textSS(m_Text);
    std::string word;

    float lineHeight = m_Font->GetLineHeight() * m_FontScale;
    float lineLevel = 0;
    float currentLineSize = 0.f;
    float wordSize = 0.f;
    float spaceAdvance = m_Font->GetSpaceAdvance() * m_FontScale;

    for (unsigned i = 0; i < m_MaxLinesNumber; ++i)
    {
        lines.push_back(std::string());
        lineLevel = i * lineHeight;
        currentLineSize = 0;

        while ((word[0] != 0) || textSS >> word)
        {
            wordSize = GetTextSize(word);
            if ((currentLineSize + wordSize + spaceAdvance) > m_MaxLineSize)
            {
                break;
            }

            currentLineSize += wordSize + spaceAdvance;
            lines.back() += word + " ";
            word[0] = 0;
        }
    }
    return lines;
}

Component* TextRender::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    Transform* transform = entity->GetComponent<Transform>();
    if (!transform)
    {
        Debug::Log(Debug::DebugLevel::Error, "Cannot create TextRender without Transform");
        return nullptr;
    }

    bool wrapLines = false;
    std::string text = node.attribute("text").value();
    Font* font = nullptr;
    glm::vec3 color(1.f);
    unsigned fontSize = 0;

    if (!ParseBoolFromXMLNode(node, "wrapLines", wrapLines, false) ||
        !ParseFontFromXMLNode(node, &font) ||
        !ParseVec3FromXMLNode(node, "color", color, false) ||
        !ParseUintFromXMLNode(node, "fontSize", fontSize, true))
    {
        return nullptr;
    }

    return new TextRender(font, fontSize, color, wrapLines, transform->GetScale(), text);
}

Component* TextRender::Create(Font* font, const unsigned& fontSize, const glm::vec3& color, const bool& input, const bool& wrapLines,
    const glm::vec3& size, const std::string& text)
{
    return new TextRender(font, fontSize, color, wrapLines, size, text);
}
