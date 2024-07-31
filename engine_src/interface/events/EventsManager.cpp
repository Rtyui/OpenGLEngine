#include "interface/events/EventsManager.h"
#include "interface/Debug.h"

EventsManager* EventsManager::s_EventsManager = nullptr;

EventsManager* EventsManager::Singleton()
{
    if (!s_EventsManager)
    {
        s_EventsManager = new EventsManager();
    }

    return s_EventsManager;
}

void EventsManager::Destroy()
{
    if (!s_EventsManager)
    {
        return;
    }
    delete s_EventsManager;
}

void EventsManager::RegisterListener(EventListener* eventListener)
{
    m_RegisteredListeners.push_back(eventListener);
}

void EventsManager::RegisterListener(EventListener* eventListener, Event::EventType eventType)
{
    if (m_SpecificListeners.find(eventType) == m_SpecificListeners.end())
    {
        m_SpecificListeners[eventType] = std::vector<EventListener*>();
    }
    m_SpecificListeners[eventType].push_back(eventListener);
}

void EventsManager::RegisterEvent(Event* event)
{
    for (EventListener* eventListener : m_RegisteredListeners)
    {
        event->GetHandled(eventListener);
    }

    if (m_SpecificListeners.find(event->GetType()) != m_SpecificListeners.end())
    {
        for (EventListener* eventListener : m_SpecificListeners[event->GetType()])
        {
            event->GetHandled(eventListener);
        }
    }

    delete event;
}
