#include "interface/Console.h"
#include "components/Transform.h"
#include "ui/components/InputTextRender.h"
#include "ui/components/SpriteRender.h"
#include "interface/Display.h"
#include "Resources.h"
#include "rendering/Renderer.h"
#include "interface/events/KeyEvent.h"
#include "interface/Debug.h"
#include "interface/ConsoleCommands.h"

const unsigned Console::NUM_ROWS = 60;
const unsigned Console::HISTORY_SIZE = 10;
Console* Console:: s_Console = nullptr;


Console::Console() :
    EventListener(Event::EventType::KeyEvent), m_Show(false), m_HistoryIndex(-1), m_History(HISTORY_SIZE)
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
        bool success = ConsoleCommands::ExecuteCommand(textRender->GetText());
        if (success)
        {
            for (unsigned i = (HISTORY_SIZE - 1); i > 0; --i)
            {
                m_History[i] = m_History[i - 1];
            }
            m_History[0] = textRender->GetText();
        }
        textRender->SetText("");
        m_HistoryIndex = -1;
        return;
    }

    if ((keyEvent->GetKey() == GLFW_KEY_TAB) && (keyEvent->GetAction() == GLFW_PRESS) && !keyEvent->GetMods())
    {
        TextRender* textRender = m_InputRow->GetComponent<TextRender>();
        std::vector<ConsoleCommands::ConsoleCommand> commands = ConsoleCommands::GetCommandsWithPrefix(textRender->GetText());

        if (commands.size() == 1)
        {
            textRender->SetText(commands[0].m_Command);
        }

        for (ConsoleCommands::ConsoleCommand command : commands)
        {
            PushLine(command.m_CommandUsage);

        }
    }

    if ((keyEvent->GetKey() == GLFW_KEY_UP) && (keyEvent->GetAction() == GLFW_PRESS) && !keyEvent->GetMods())
    {
        TextRender* textRender = m_InputRow->GetComponent<TextRender>();

        if (m_HistoryIndex == (HISTORY_SIZE - 1))
        {
            return;
        }

        if (!m_History[m_HistoryIndex + 1][0])
        {
            return;
        }

        ++m_HistoryIndex;
        textRender->SetText(m_History[m_HistoryIndex]);
    }

    if ((keyEvent->GetKey() == GLFW_KEY_DOWN) && (keyEvent->GetAction() == GLFW_PRESS) && !keyEvent->GetMods())
    {
        TextRender* textRender = m_InputRow->GetComponent<TextRender>();

        if (m_HistoryIndex < 0)
        {
            return;
        }

        if (m_HistoryIndex == 0)
        {
            textRender->SetText("");
            m_HistoryIndex = -1;
            return;
        }

        --m_HistoryIndex;
        textRender->SetText(m_History[m_HistoryIndex]);
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

    m_InputRow->GetComponent<InputTextRender>()->SetSelected(m_Show);
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
    Transform* transform = nullptr;
    TextRender* textRender = nullptr;

    if (text.find('\n') != std::string::npos)
    {
        Debug::Log(Debug::DebugLevel::Warning, "Console debug with newline not implemented yet!");
        return;
    }

    for (Entity* entity : m_Rows)
    {
        transform = entity->GetComponent<Transform>();
        if ((transform->GetPosition().y - m_RowHeight) < 0.f)
        {
            textRender = entity->GetComponent<TextRender>();
            textRender->SetText(text);
            textRender->SetColor(color);
            transform->IncreasePosition(glm::vec3(0.f, m_RowHeight * (NUM_ROWS - 2), 0.f));
        }
        else
        {
            transform->IncreasePosition(glm::vec3(0.f, -m_RowHeight, 0.f));
        }
    }
}

void Console::CreateTextRows()
{
    float displayWidth = (float)Display::Singleton()->GetWidth();
    std::string name("");

    Entity* entity = nullptr;
    Transform* transform = nullptr;
    TextRender* textRender = nullptr;

    for (unsigned i = 0; i < NUM_ROWS - 1; ++i)
    {
        name = std::string("ConsoleRow") + std::to_string(i);
        entity = Entity::Create(name);
        m_Rows.push_back(entity);

        transform = static_cast<Transform*>(Transform::Create(glm::vec3(0.f, i * m_RowHeight, 0.f), glm::vec3(0.f), glm::vec3(displayWidth, m_RowHeight, 1.f), true));
        entity->AddComponent(transform);

        textRender = static_cast<TextRender*>(TextRender::Create(m_Font, 0, glm::vec3(1.f, 1.f, 1.f), false, false, transform->GetScale(), name));
        entity->AddComponent(textRender);

        Renderer::Singleton()->Submit(static_cast<Renderable*>(textRender));
    }

    name = std::string("ConsoleRow") + std::to_string(NUM_ROWS - 1);
    m_InputRow = Entity::Create(name);

    transform = static_cast<Transform*>(Transform::Create(glm::vec3(0.f, (NUM_ROWS - 1) * m_RowHeight, 0.f), glm::vec3(0.f), glm::vec3(displayWidth, m_RowHeight, 1.f), true));
    m_InputRow->AddComponent(transform);

    textRender = static_cast<InputTextRender*>(InputTextRender::Create(m_Font, glm::vec3(1.f, 1.f, 1.f), transform->GetScale()));
    m_InputRow->AddComponent(textRender);

    Renderer::Singleton()->Submit(static_cast<Renderable*>(textRender));
}

void Console::CreateBackground()
{
    m_Background = Entity::Create("ConsoleBackground");
    Transform* transform = static_cast<Transform*>(Transform::Create(glm::vec3(0.f), glm::vec3(0.f),
        glm::vec3((float)Display::Singleton()->GetWidth(), (float)Display::Singleton()->GetHeight(), 1.f), true));
    m_Background->AddComponent(transform);

    SpriteRender* spriteRender = static_cast<SpriteRender*>(SpriteRender::Create(Resources::Singleton()->GetTexture("white"), glm::vec4(0.f, 0.f, 0.f, 0.7f)));
    m_Background->AddComponent(spriteRender);
    
    Renderer::Singleton()->Submit(static_cast<Renderable*>(spriteRender));
}
