#include "interface/Display.h"
#include "interface/Debug.h"
#include "interface/events/WindowSizeEvent.h"

Display* Display::s_Display = nullptr;

void Display::Destroy()
{
    if (!s_Display)
    {
        return;
    }
    delete s_Display;
}

Display::Display() :
    EventListener(Event::EventType::WindowSizeEvent), m_Width(1920), m_Height(1080),
    m_OriginalWidth(1920), m_OriginalHeight(1080), m_WidthScale(1.f), m_HeightScale(1.f)
{
    if (!glfwInit())
    {
        Debug::Log(Debug::Error, "Could not init glfw!");
        exit(-1);
    }

    m_Window = glfwCreateWindow(m_Width, m_Height, "Hello World", NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        Debug::Log(Debug::Error, "Null window!");
        exit(-1);
    }

    glfwMakeContextCurrent(m_Window);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        Debug::Log(Debug::Error, "Could not init glew!");
        exit(-1);
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    Debug::RegisterGLErrorCallback();
}

Display::~Display()
{
    glfwTerminate();
}

Display* Display::Singleton()
{
    if (!s_Display)
    {
        s_Display = new Display();
    }

    return s_Display;
}

const bool Display::WindowShouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}

void Display::Update() const
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Display::HandleEvent(WindowSizeEvent* event)
{
    m_Width = event->GetWidth();
    m_Height = event->GetHeight();
    m_WidthScale = (float)m_Width / m_OriginalWidth;
    m_HeightScale = (float)m_Height / m_OriginalHeight;
    //glViewport(0, 0, m_Width, m_Height);
    Debug::Log(Debug::DebugLevel::Info, "WidthScale " + std::to_string(m_WidthScale) + " " + "HeightScale " + std::to_string(m_HeightScale));
}
