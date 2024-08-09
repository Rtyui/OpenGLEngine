#include "ui/components/InputTextRender.h"
#include "components/Transform.h"
#include "interface/events/CharEvent.h"
#include "interface/events/KeyEvent.h"
#include "interface/events/MouseButtonEvent.h"
#include "interface/TimeClock.h"
#include "interface/Debug.h"
#include "interface/Input.h"
#include "utils/XMLUtils.h"
#include "GLFW/glfw3.h"

#include <vendor/glm/gtc/matrix_transform.hpp>

const float InputTextRender::CURSOR_FLASH_INTERVAL = 0.5f;
float InputTextRender::s_CursorWidth = 4.f;
InputTextRender* InputTextRender::s_SelectedInput = nullptr;

InputTextRender::InputTextRender(Font* font, const glm::vec3& color, const glm::vec3& size) :
    EventListener({ Event::EventType::CharEvent, Event::EventType::KeyEvent, Event::EventType::MouseButtonEvent }),
    TextRender(font, color), m_Cursor(0.f), m_CursorTimer(0.f), m_RenderCursor(false), m_Selected(false)
{
    Initialize(size);
}

void InputTextRender::Initialize(const glm::vec3& size)
{
    TextRender::Initialize(size);
    CreateCursorMesh();
    UpdateCursorTM(0);
}

void InputTextRender::UpdateCursorAnimation()
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

InputTextRender::~InputTextRender()
{
    delete m_CursorMesh;
}

void InputTextRender::Update()
{
    UpdateCursorAnimation();
}

void InputTextRender::SetText(const std::string& text)
{
    TextRender::SetText(text);
    m_Cursor = InputTextRender::GetTextSize(text);
    UpdateCursorTM(0);
}

void InputTextRender::SetSelected(const bool& selected)
{
    if (m_Selected == selected)
    {
        return;
    }

    m_Selected = selected;
    m_RenderCursor = selected;

    if (selected)
        Debug::Log(Debug::DebugLevel::Info, "Selecting box " + m_Entity->GetName());

    if (selected)
    {
        if (s_SelectedInput)
        {
            s_SelectedInput->m_Selected = false;
            s_SelectedInput->m_RenderCursor = false;
        }
        s_SelectedInput = this;
    }
    else
    {
        if (s_SelectedInput == this)
        {
            s_SelectedInput = nullptr;
        }
    }
}

void InputTextRender::HandleEvent(KeyEvent* keyEvent)
{
    if (!IsActive())
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

    if (m_Text.size() <= 0)
    {
        return;
    }

    if (m_Text.back() != ' ')
    {
        m_Mesh->PopData(16, 6);
    }

    FontCharacter* fontCharacter = m_Font->GetCharacter(m_Text.back());
    m_Cursor -= fontCharacter->m_Advance * m_FontScale;
    m_Text.pop_back();
    UpdateCursorTM(0);
    return;
}

void InputTextRender::HandleEvent(CharEvent* charEvent)
{
    if (!IsActive())
    {
        return;
    }

    if (!m_Selected)
    {
        return;
    }

    std::vector<float> vertices;
    std::vector<unsigned> indices;

    if (AddCharacter(vertices, indices, charEvent->GetCodepoint(), 0, (m_Mesh->GetIndexBuffer()->GetCount() / 6) * 4, m_Cursor))
    {
        m_Text += (char)charEvent->GetCodepoint();
        m_Mesh->PushData(vertices, indices);
        UpdateCursorTM(0);
    }
    return;
}

void InputTextRender::HandleEvent(MouseButtonEvent* mouseButtonEvent)
{
    if (!IsActive())
    {
        return;
    }

    if ((mouseButtonEvent->GetAction() != GLFW_PRESS) || (mouseButtonEvent->GetKey() != GLFW_MOUSE_BUTTON_LEFT))
    {
        return;
    }
    glm::vec2 mousePosition = (glm::vec2)Input::Singleton()->GetMousePosition();

    SetSelected(m_Entity->GetTransform()->IsScreenPointOn(mousePosition));
}

void InputTextRender::InitTextParams(const glm::vec3& size, unsigned& maxCharCount)
{
    m_FontScale = size.y / m_Font->GetLineHeight();
    maxCharCount = (unsigned)(size.x / (m_Font->GetSmallestCharSize() * m_FontScale));
}

void InputTextRender::CreateCursorMesh()
{
    VertexBufferLayout* cursorLayout = new VertexBufferLayout();
    cursorLayout->Push<float>("position", 2);

    m_CursorMesh = Mesh::CreateEmptyMesh(8, cursorLayout, 6, "textCursor");

    std::vector<unsigned> indices = { 0, 1, 2, 2, 1, 3 };
    m_CursorMesh->SetData(indices);

    std::vector<float> vertices;

    float lineHeight = m_Font->GetLineHeight() * m_FontScale;

    vertices.push_back(0.f);
    vertices.push_back(0.1f * lineHeight);

    vertices.push_back(0.f);
    vertices.push_back(0.9f * lineHeight);

    vertices.push_back(s_CursorWidth * m_FontScale);
    vertices.push_back(0.1f * lineHeight);

    vertices.push_back(s_CursorWidth * m_FontScale);
    vertices.push_back(0.9f * lineHeight);

    m_CursorMesh->SetData(vertices);
}

void InputTextRender::UpdateCursorTM(const unsigned& lineIndex)
{
    float lineHeight = m_Font->GetLineHeight() * m_FontScale;
    float lineLevel = lineIndex * lineHeight;
    /*std::vector<float> vertices;


    vertices.push_back(m_Cursor);
    vertices.push_back(lineLevel + 0.1f * lineHeight);

    vertices.push_back(m_Cursor);
    vertices.push_back(lineLevel + 0.9f * lineHeight);

    vertices.push_back(m_Cursor + s_CursorWidth * m_FontScale);
    vertices.push_back(lineLevel + 0.1f * lineHeight);

    vertices.push_back(m_Cursor + s_CursorWidth * m_FontScale);
    vertices.push_back(lineLevel + 0.9f * lineHeight);

    m_CursorMesh->SetData(vertices);*/
    m_CursorTransform = glm::translate(glm::mat4(1.f), glm::vec3(m_Cursor, lineLevel, 0.f));
}

Component* InputTextRender::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    Transform* transform = entity->GetComponent<Transform>();
    if (!transform)
    {
        Debug::Log(Debug::DebugLevel::Error, "Cannot create TextRender without Transform");
        return nullptr;
    }

    Font* font = nullptr;
    glm::vec3 color(1.f);

    if (!ParseFontFromXMLNode(node, &font) ||
        !ParseVec3FromXMLNode(node, "color", color, false))
    {
        return nullptr;
    }

    return new InputTextRender(font, color, transform->GetScale());
}

Component* InputTextRender::Create(Font* font, const glm::vec3& color, const glm::vec3& size)
{
    return new InputTextRender(font, color, size);
}
