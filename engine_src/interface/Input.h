#pragma once
#include "interface/Display.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <vendor/glm/glm.hpp>

class Input
{
public:

    enum class KeyState
    {
        JustPressed,
        Pressed,
        JustReleased,
        Released
    };


private:

    static Input* s_Input;

    Input();

public:

    static Input* Singleton();
    static void Destroy();

    bool GetKey(int key);
    glm::dvec2 GetMousePosition();
    void Update();

private:

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void CharCallback(GLFWwindow* window, unsigned int codepoint);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

};

