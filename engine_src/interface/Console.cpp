#include "interface/Console.h"
#include "components/TransformUI.h"
#include "components/TextRender.h"
#include "components/SpriteRender.h"
#include "interface/Display.h"
#include "Resources.h"
#include "rendering/Renderer.h"
#include "interface/events/KeyEvent.h"
#include "interface/Debug.h"
#include "interface/ConsoleCommands.h"

const unsigned Console::NUM_ROWS = 30;
Console* Console:: s_Console = nullptr;


Console::Console() :
    EventListener(Event::EventType::KeyEvent), m_Show(false)
{
    m_Font = Resources::Singleton()->GetFont("dialog");
    m_RowHeight = (float)Display::Singleton()->GetHeight() / NUM_ROWS;

    CreateBackground();
    CreateTextRows();

    UpdateShow();
}

Console::~Console()
{
    for (Entity* entity : m_Rows)
    {
        delete entity;
    }
    delete m_InputRow;
}

void Console::HandleEvent(KeyEvent* keyEvent)
{
    if ((keyEvent->GetKey() == GLFW_KEY_GRAVE_ACCENT) && (keyEvent->GetAction() == GLFW_PRESS) && !keyEvent->GetMods())
    {
        m_Show = !m_Show;
        if (m_Show)
        {
            return;
        }

        UpdateShow();
    }

    if (!m_Show)
    {
        return;
    }

    if ((keyEvent->GetKey() == GLFW_KEY_ENTER) && (keyEvent->GetAction() == GLFW_PRESS) && !keyEvent->GetMods())
    {
        TextRender* textRender = m_InputRow->GetComponent<TextRender>();
        //PushLine(textRender->GetText());
        ConsoleCommands::ExecuteCommand(textRender->GetText());
        textRender->SetText("");
    }
}

Console* Console::Singleton()
{
    if (!s_Console)
    {
        s_Console = new Console();
    }
    return s_Console;

}

void Console::Destroy()
{
    if (!s_Console)
    {
        return;
    }
    delete s_Console;
}

void Console::Update()
{
    if (!m_Show)
    {
        return;
    }

    if (!m_InputRow->IsActive())
    {
        UpdateShow();
    }

    UpdateEntities();
}

void Console::UpdateShow()
{
    for (Entity* entity : m_Rows)
    {
        entity->SetActive(m_Show);
    }
    m_InputRow->SetActive(m_Show);
    m_Background->SetActive(m_Show);

    m_InputRow->GetComponent<TextRender>()->SetSelected(m_Show);
}

void Console::UpdateEntities()
{
    for (Entity* entity : m_Rows)
    {
        entity->Update();
    }
    m_InputRow->Update();
}

void Console::PushLine(const std::string& text, const glm::vec3 color)
{
    TransformUI* transform = nullptr;
    TextRender* textRender = nullptr;
    for (Entity* entity : m_Rows)
    {
        transform = entity->GetComponent<TransformUI>();
        if ((transform->GetPosition().y - m_RowHeight) < 0.f)
        {
            textRender = entity->GetComponent<TextRender>();
            textRender->SetText(text);
            textRender->SetColor(color);
            transform->IncreasePosition(glm::vec2(0.f, m_RowHeight * (NUM_ROWS - 2)));
        }
        else
        {
            transform->IncreasePosition(glm::vec2(0.f, -m_RowHeight));
        }
    }
}

void Console::CreateTextRows()
{
    float displayWidth = (float)Display::Singleton()->GetWidth();
    std::string name("");

    Entity* entity = nullptr;
    TransformUI* transformUI = nullptr;
    TextRender* textRender = nullptr;

    for (unsigned i = 0; i < NUM_ROWS - 1; ++i)
    {
        name = std::string("ConsoleRow") + std::to_string(i);
        entity = Entity::Create(name);
        m_Rows.push_back(entity);

        transformUI = static_cast<TransformUI*>(TransformUI::Create(glm::vec2(0.f, i * m_RowHeight), 0.f, glm::vec2(displayWidth, m_RowHeight)));
        entity->AddComponent(transformUI);

        textRender = static_cast<TextRender*>(TextRender::Create(m_Font, 0, glm::vec3(1.f, 1.f, 1.f), false, false, transformUI->GetSize(), name));
        entity->AddComponent(textRender);

        Renderer::Singleton()->Submit(textRender);
    }

    name = std::string("ConsoleRow") + std::to_string(NUM_ROWS - 1);
    m_InputRow = Entity::Create(name);

    transformUI = static_cast<TransformUI*>(TransformUI::Create(glm::vec2(0.f, (NUM_ROWS - 1) * m_RowHeight), 0.f, glm::vec2(displayWidth, m_RowHeight)));
    m_InputRow->AddComponent(transformUI);

    textRender = static_cast<TextRender*>(TextRender::Create(m_Font, 0, glm::vec3(1.f, 1.f, 1.f), true, false, transformUI->GetSize(), ""));
    m_InputRow->AddComponent(textRender);

    Renderer::Singleton()->Submit(textRender);
}

void Console::CreateBackground()
{
    m_Background = Entity::Create("ConsoleBackground");
    TransformUI* transformUI =
        static_cast<TransformUI*>(TransformUI::Create(glm::vec2(0.f, 0.f), 0.f,
                                                        glm::vec2((float)Display::Singleton()->GetWidth(), (float)Display::Singleton()->GetHeight())));
    m_Background->AddComponent(transformUI);

    SpriteRender* spriteRender = static_cast<SpriteRender*>(SpriteRender::Create(Resources::Singleton()->GetTexture("white"), glm::vec4(0.f, 0.f, 0.f, 0.7f)));
    m_Background->AddComponent(spriteRender);
    
    Renderer::Singleton()->Submit(spriteRender);
}
