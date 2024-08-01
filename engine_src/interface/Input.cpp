#include "interface/Input.h"
#include "interface/Debug.h"
#include "interface/events/EventsManager.h"
#include "interface/events/KeyEvent.h"
#include "interface/events/CharEvent.h"
#include "interface/events/MouseButtonEvent.h"
#include "interface/events/WindowSizeEvent.h"

Input* Input::s_Input = nullptr;

Input::Input()
{
    glfwSetKeyCallback(Display::Singleton()->GetWindow(), KeyCallback);
    glfwSetCharCallback(Display::Singleton()->GetWindow(), CharCallback);
    glfwSetMouseButtonCallback(Display::Singleton()->GetWindow(), MouseButtonCallback);
    glfwSetWindowSizeCallback(Display::Singleton()->GetWindow(), WindowSizeCallback);
}

Input* Input::Singleton()
{
    if (!s_Input)
    {
        s_Input = new Input();
    }
    return s_Input;
}

void Input::Destroy()
{
    if (!s_Input)
    {
        return;
    }
    delete s_Input;
}

bool Input::GetKey(int key)
{
    if (!Display::Singleton())
    {
        Debug::Log(Debug::Error, "display NULL!");
        return false;
    }

    int state = glfwGetKey(Display::Singleton()->GetWindow(), key);
    if (state == GLFW_RELEASE)
        return false;
    return true;
}

glm::dvec2 Input::GetMousePosition()
{
    glm::dvec2 position(0.f);
    glfwGetCursorPos(Display::Singleton()->GetWindow(), &position.x, &position.y);
    return position;
}

void Input::Update()
{
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    EventsManager::Singleton()->RegisterEvent(new KeyEvent(key, scancode, action, mods));
}

void Input::CharCallback(GLFWwindow* window, unsigned int codepoint)
{
    EventsManager::Singleton()->RegisterEvent(new CharEvent(codepoint));
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    EventsManager::Singleton()->RegisterEvent(new MouseButtonEvent(button, action, mods));
}

void Input::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    EventsManager::Singleton()->RegisterEvent(new WindowSizeEvent(width, height));
}
