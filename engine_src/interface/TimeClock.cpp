#include "interface/TimeClock.h"

#include <GLFW/glfw3.h>

TimeClock* TimeClock::s_TimeClock = nullptr;

TimeClock::TimeClock() :
    m_DeltaTime(0.f)
{
    m_LastTime = (float)glfwGetTime();
}

TimeClock* TimeClock::Singleton()
{
    if (!s_TimeClock)
    {
        s_TimeClock = new TimeClock();
    }
    return s_TimeClock;
}

void TimeClock::Destroy()
{
    if (!s_TimeClock)
    {
        return;
    }
    delete s_TimeClock;
}

void TimeClock::Update()
{
    float currentTime = (float)glfwGetTime();
    m_DeltaTime = currentTime - m_LastTime;
    m_LastTime = currentTime;
}
