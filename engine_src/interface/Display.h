#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Input;
class Display
{
private:
    static Display* s_Display;

    GLFWwindow* m_Window;
    int m_Width;
    int m_Height;
public:
    ~Display();

    static Display* Singleton();
    static void Destroy();

    const bool WindowShouldClose() const;
    void Update() const;

    inline const int GetWidth() const { return m_Width; }
    inline const int GetHeight() const { return m_Height; }
    inline GLFWwindow* GetWindow() const { return m_Window; }


private:

    Display();
};
