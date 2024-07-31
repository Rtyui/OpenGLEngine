#include "interface/events/KeyEvent.h"
#include "interface/events/EventListener.h"

void KeyEvent::GetHandled(EventListener* eventListener)
{
    eventListener->HandleEvent(this);
}

KeyEvent::KeyEvent(int key, int scancode, int action, int mods) :
    m_Key(key), m_Scancode(scancode), m_Action(action), m_Mods(mods)
{
}

Event::EventType KeyEvent::GetType()
{
    return EventType::KeyEvent;
}
