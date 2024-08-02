#include "components/TextRender.h"
#include "interface/Debug.h"
#include "Resources.h"
#include "components/TransformUI.h"
#include "interface/events/CharEvent.h"
#include "interface/events/KeyEvent.h"
#include "interface/TimeClock.h"
#include "interface/events/MouseButtonEvent.h"
#include "interface/Input.h"

#include <sstream>

const float TextRender::CURSOR_FLASH_INTERVAL = 0.5f;
float TextRender::s_CursorWidth = 4.f;
TextRender* TextRender::s_SelectedTextRender = nullptr;


TextRender::TextRender() :
    m_OriginalText(""), m_MeshText(""), m_Font(nullptr), m_Mesh(nullptr), m_CursorMesh(nullptr), m_Color(1.f), m_FontSize(10),
    m_FontScale(0), m_Cursor(0.f), m_Input(false), m_RenderCursor(false), m_Selected(false), m_MaxLineSize(0), m_WrapLines(false),
    m_MaxLinesNumber(0), m_MeshIndex(0), m_CursorTimer(0.f)
{
}

TextRender::TextRender(Font* font, const unsigned& fontSize, const glm::vec3& color, const bool& input, const bool& wrapLines,
    const glm::vec3& size, const std::string& text) :
    EventListener({ Event::EventType::CharEvent, Event::EventType::KeyEvent, Event::EventType::MouseButtonEvent }),
    m_OriginalText(text), m_MeshText(""), m_Mesh(nullptr), m_CursorMesh(nullptr), m_Font(font), m_Color(color),
    m_FontSize(fontSize), m_FontScale(0), m_Input(input), m_RenderCursor(false), m_Selected(false), m_WrapLines(wrapLines), m_MeshIndex(0),
    m_CursorTimer(0.f)
{
    Initialize(size);
}

TextRender::~TextRender()
{
    delete m_Mesh;
}

void TextRender::HandleEvent(KeyEvent* keyEvent)
{
    if (!IsActive())
    {
        return;
    }

    if (!m_Input)
    {
        return;
    }

    if (!m_Selected)
    {
        return;
    }

    if ((keyEvent->GetAction() != GLFW_PRESS) && (keyEvent->GetAction() != GLFW_REPEAT))
    {
        return;
    }

    if (keyEvent->GetKey() != GLFW_KEY_BACKSPACE)
    {
        return;
    }

    if (m_MeshText.size() <= 0)
    {
        return;
    }

    if (m_MeshText.back() != ' ')
    {
        m_MeshIndex -= 4;
        m_Mesh->PopData(16, 6);
    }

    FontCharacter* fontCharacter = m_Font->GetCharacter(m_MeshText.back());
    m_Cursor -= fontCharacter->m_Advance * m_FontScale;
    m_MeshText.pop_back();
    UpdateMeshTextCursor(0);
    return;
}

void TextRender::HandleEvent(CharEvent* charEvent)
{
    if (!IsActive())
    {
        return;
    }

    if (!m_Input)
    {
        return;
    }

    if (!m_Selected)
    {
        return;
    }

    std::vector<float> vertices;
    std::vector<unsigned> indices;

    if (AddCharacter(vertices, indices, charEvent->GetCodepoint(), 0))
    {
        m_MeshText += (char)charEvent->GetCodepoint();
        m_Mesh->PushData(vertices, indices);
    }
    return;
}

void TextRender::HandleEvent(MouseButtonEvent* mouseButtonEvent)
{
    if (!IsActive())
    {
        return;
    }

    if (!m_Input)
    {
        return;
    }

    if ((mouseButtonEvent->GetAction() != GLFW_PRESS) || (mouseButtonEvent->GetKey() != GLFW_MOUSE_BUTTON_LEFT))
    {
        return;
    }
    glm::vec2 mousePosition = (glm::vec2)Input::Singleton()->GetMousePosition();

    TransformUI* transform = static_cast<TransformUI*>(m_Entity->GetTransform());

    SetSelected(transform->IsScreenPointOn(mousePosition));
}

void TextRender::GetSubmitted()
{
    Renderer::Singleton()->Submit(this);
}

void TextRender::GetUnSubmitted()
{
    Renderer::Singleton()->UnSubmit(this);
}

void TextRender::Update()
{
    UpdateCursorAnimation();
}

void TextRender::Initialize(const glm::vec3& size)
{
    unsigned maxCharCount = 0;
    m_MaxLineSize = size.x;
    if (m_WrapLines && !m_Input)
    {
        m_FontScale = (float)m_FontSize / m_Font->GetBaseFontSize();
        m_MaxLinesNumber = (unsigned)(size.y / (m_Font->GetLineHeight() * m_FontScale));
        m_FontScale = (float)m_FontSize / m_Font->GetBaseFontSize();
        maxCharCount = (unsigned)(m_MaxLineSize  / (m_Font->GetSmallestCharSize() * m_FontScale)) * m_MaxLinesNumber;
    }
    else
    {
        m_FontScale = size.y / m_Font->GetLineHeight();
        m_MaxLinesNumber = 1;
        m_FontScale = size.y / m_Font->GetLineHeight();
        maxCharCount = (unsigned)(m_MaxLineSize / (m_Font->GetSmallestCharSize() * m_FontScale));
    }

    unsigned verticesSize = maxCharCount * 16;
    unsigned indicesSize = maxCharCount * 6;

    VertexBufferLayout* layout = new VertexBufferLayout();
    layout->Push<float>("position", 2);
    layout->Push<float>("texCoord", 2);

    m_Mesh = Mesh::CreateEmptyMesh(verticesSize, layout, indicesSize, "textMesh");

    if (m_Input)
    {
        VertexBufferLayout* cursorLayout = new VertexBufferLayout();
        cursorLayout->Push<float>("position", 2);
    
        m_CursorMesh = Mesh::CreateEmptyMesh(8, cursorLayout, 6, "textCursor");

        std::vector<unsigned> indices = { 0, 1, 2, 2, 1, 3 };
        m_CursorMesh->SetData(indices);

        UpdateMeshTextCursor(0);
    }
    UpdateMeshText();

}

bool TextRender::AddCharacter(std::vector<float>& vertices, std::vector<unsigned>& indices, const char& c, const unsigned& lineIndex)
{
    FontCharacter* fontCharacter = m_Font->GetCharacter(c);
    float lineLevel = lineIndex * m_Font->GetLineHeight() * m_FontScale;

    if (!fontCharacter)
    {
        Debug::Log(Debug::Warning, "Font does not have character " + c);
        return false;
    }

    if ((m_Cursor + fontCharacter->m_Advance * m_FontScale) > m_MaxLineSize)
    {
        Debug::Log(Debug::Warning, "End of line reached at " + std::to_string(m_MeshText.size()) + " chars!");
        return false;
    }

    if (c == ' ')
    {
        m_Cursor += fontCharacter->m_Advance * m_FontScale;
        UpdateMeshTextCursor(lineIndex);
        return true;
    }

    // Top left position
    vertices.push_back(m_Cursor + fontCharacter->m_Offset.x * m_FontScale);
    vertices.push_back(lineLevel + fontCharacter->m_Offset.y * m_FontScale);

    // Top left tex coord
    vertices.push_back(fontCharacter->m_TexCoord.x);
    vertices.push_back(fontCharacter->m_TexCoord.y);

    // Bottom left position
    vertices.push_back(m_Cursor + fontCharacter->m_Offset.x * m_FontScale);
    vertices.push_back(lineLevel + (fontCharacter->m_Offset.y + fontCharacter->m_ScreenSize.y) * m_FontScale);

    // Bottom left tex coord
    vertices.push_back(fontCharacter->m_TexCoord.x);
    vertices.push_back(fontCharacter->m_TexCoord.y + fontCharacter->m_TextureSize.y);

    // Top right position
    vertices.push_back(m_Cursor + (fontCharacter->m_Offset.x + fontCharacter->m_ScreenSize.x) * m_FontScale);
    vertices.push_back(lineLevel + fontCharacter->m_Offset.y * m_FontScale);

    // Top right tex coord
    vertices.push_back(fontCharacter->m_TexCoord.x + fontCharacter->m_TextureSize.x);
    vertices.push_back(fontCharacter->m_TexCoord.y);

    // Bottom right position
    vertices.push_back(m_Cursor + (fontCharacter->m_Offset.x + fontCharacter->m_ScreenSize.x) * m_FontScale);
    vertices.push_back(lineLevel + (fontCharacter->m_Offset.y + fontCharacter->m_ScreenSize.y) * m_FontScale);

    // Bottom right tex coord
    vertices.push_back(fontCharacter->m_TexCoord.x + fontCharacter->m_TextureSize.x);
    vertices.push_back(fontCharacter->m_TexCoord.y + fontCharacter->m_TextureSize.y);

    indices.push_back(m_MeshIndex);
    indices.push_back(m_MeshIndex + 1);
    indices.push_back(m_MeshIndex + 2);
    indices.push_back(m_MeshIndex + 2);
    indices.push_back(m_MeshIndex + 1);
    indices.push_back(m_MeshIndex + 3);

    m_Cursor += fontCharacter->m_Advance * m_FontScale;
    m_MeshIndex += 4;
    UpdateMeshTextCursor(lineIndex);
    return true;
}

void TextRender::UpdateMeshText()
{
    std::vector<float> vertices;
    std::vector<unsigned> indices;
    unsigned i = 0;
    m_MeshIndex = 0;
    m_Cursor = 0.f;
    m_MeshText = "";

    if (!m_OriginalText[0])
    {
        m_Mesh->Empty();
        UpdateMeshTextCursor(0);
        return;
    }

    if (m_WrapLines && (m_MaxLinesNumber > 1))
    {
        std::vector<std::string> lines = GetTextLines();
        for (i = 0; i < lines.size() && lines[i][0]; ++i)
        {
            for (char& c : lines[i])
            {
                if (!AddCharacter(vertices, indices, c, i))
                {
                    Debug::Log(Debug::Error, std::string("Cannot add character '") + c + "'!");
                    return;
                }
                m_MeshText += c;
            }
            m_Cursor = 0;
            m_MeshText += '\n';
        }
    }
    else
    {
        for (char& c : m_OriginalText)
        {
            if (!AddCharacter(vertices, indices, c, 0))
            {
                Debug::Log(Debug::Verbose, std::string("Stopping at character '") + c + "'!");
                break;
            }
            m_MeshText += c;
        }
    }

    m_Mesh->SetData(vertices, indices);
}

void TextRender::UpdateMeshTextCursor(const unsigned& lineIndex)
{
    if (!m_Input)
    {
        return;
    }

    std::vector<float> vertices;

    float lineHeight = m_Font->GetLineHeight() * m_FontScale;
    float lineLevel = lineIndex * lineHeight;

    vertices.push_back(m_Cursor);
    vertices.push_back(lineLevel + 0.1f * lineHeight);

    vertices.push_back(m_Cursor);
    vertices.push_back(lineLevel + 0.9f * lineHeight);

    vertices.push_back(m_Cursor + s_CursorWidth * m_FontScale);
    vertices.push_back(lineLevel + 0.1f * lineHeight);

    vertices.push_back(m_Cursor + s_CursorWidth * m_FontScale);
    vertices.push_back(lineLevel + 0.9f * lineHeight);

    m_CursorMesh->SetData(vertices);
}

std::vector<std::string> TextRender::GetTextLines()
{
    std::vector<std::string> lines;

    std::stringstream textSS(m_OriginalText);
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
            wordSize = GetWordSize(word);
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

void TextRender::SetText(const std::string& text)
{
    m_OriginalText = text;
    UpdateMeshText();
}

void TextRender::SetSelected(const bool& selected)
{
    if (m_Selected == selected)
    {
        return;
    }

    m_Selected = selected;
    m_RenderCursor = selected;

    if(selected)
        Debug::LogConsole(Debug::Info, "Selecting box " + m_Entity->GetName());
    
    if (selected)
    {
        if (s_SelectedTextRender)
        {
            s_SelectedTextRender->m_Selected = false;
            s_SelectedTextRender->m_RenderCursor = false;
        }
        s_SelectedTextRender = this;
    }
    else
    {
        if (s_SelectedTextRender == this)
        {
            s_SelectedTextRender = nullptr;
        }
    }
}

Component* TextRender::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    Font* font;

    int temp = 0;
    const char* buffer = nullptr;

    bool input = false;
    bool wrapLines = false;
    std::string text;
    glm::vec3 color(1.f);
    unsigned fontSize = 0;

    buffer = node.attribute("input").value();
    if ((sscanf_s(buffer, "%d", &temp) != 1))
    {
        Debug::Log(Debug::Error, "Error while parsing text input '" + std::string(buffer) + "'!");
        return nullptr;
    }
    input = temp;

    buffer = node.attribute("wrapLines").value();
    if ((sscanf_s(buffer, "%d", &temp) != 1))
    {
        Debug::Log(Debug::Error, "Error while parsing text wrapLines '" + std::string(buffer) + "'!");
        return nullptr;
    }
    wrapLines = temp;

    text = node.attribute("text").value();

    buffer = node.attribute("font").value();
    font = Resources::Singleton()->GetFont(buffer);
    if (!font)
    {
        Debug::Log(Debug::Error, "Invalid font '" + std::string(buffer) + "'!");
        return nullptr;
    }

    buffer = node.attribute("color").value();
    if (buffer[0] && (sscanf_s(buffer, "%f %f %f", &color.r, &color.g, &color.b) != 3))
    {
        Debug::Log(Debug::Error, "Error while parsing text color data '" + std::string(buffer) + "'!");
        return nullptr;
    }

    buffer = node.attribute("fontSize").value();
    if (!input && (sscanf_s(buffer, "%u", &fontSize) != 1))
    {
        Debug::Log(Debug::Error, "Error while parsing text font size '" + std::string(buffer) + "'!");
        return nullptr;
    }

    TransformUI* transformUI = entity->GetComponent<TransformUI>();
    return new TextRender(font, fontSize, color, input, wrapLines, transformUI->GetScale(), text);
}

void TextRender::SetColor(const glm::vec3& color)
{
    m_Color = color;
}

Component* TextRender::Create(Font* font, const unsigned& fontSize, const glm::vec3& color, const bool& input, const bool& wrapLines,
    const glm::vec3& size, const std::string& text)
{
    return new TextRender(font, fontSize, color, input, wrapLines, size, text);
}

float TextRender::GetWordSize(const std::string& word)
{
    float size = 0.f;
    FontCharacter* fontCharacter;

    for (char c : word)
    {
        fontCharacter = m_Font->GetCharacter(c);
        size += fontCharacter->m_Advance * m_FontScale;
    }

    return size;
}

void TextRender::UpdateCursorAnimation()
{
    if (!m_Selected)
    {
        return;
    }
    m_CursorTimer += TimeClock::Singleton()->GetDeltaTime();
    if (m_CursorTimer >= CURSOR_FLASH_INTERVAL)
    {
        m_CursorTimer = 0.f;
        m_RenderCursor = !m_RenderCursor;
    }
}
