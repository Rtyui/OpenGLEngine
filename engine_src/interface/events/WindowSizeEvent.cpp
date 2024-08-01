#include "interface/events/WindowSizeEvent.h"
#include "interface/events/EventListener.h"

void WindowSizeEvent::GetHandled(EventListener* eventListener)
{
    eventListener->HandleEvent(this);
}

WindowSizeEvent::WindowSizeEvent(int width, int height) :
    m_Width(width), m_Height(height)
{
}

Event::EventType WindowSizeEvent::GetType()
{
    return Event::EventType::WindowSizeEvent;
}
