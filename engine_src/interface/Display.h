#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "events/EventListener.h"

class Input;
class Display : public EventListener
{
private:
    static Display* s_Display;

    GLFWwindow* m_Window;
    int m_Width;
    int m_Height;
    int m_OriginalWidth;
    int m_OriginalHeight;
    float m_WidthScale;
    float m_HeightScale;
public:
    ~Display();

    static Display* Singleton();
    static void Destroy();

    const bool WindowShouldClose() const;
    void Update() const;

    void HandleEvent(WindowSizeEvent* event);

    inline const int GetWidth() const { return m_Width; }
    inline const int GetHeight() const { return m_Height; }
    inline GLFWwindow* GetWindow() const { return m_Window; }
    inline const float GetWidthScale() const { return m_WidthScale; }
    inline const float GetHeightScale() const { return m_HeightScale; }

private:

    Display();
};
