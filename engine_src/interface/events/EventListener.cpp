#include "interface/events/EventListener.h"
#include "interface/events/EventsManager.h"

EventListener::EventListener()
{
    EventsManager::Singleton()->RegisterListener(this);
}

EventListener::EventListener(const bool& shouldRegister)
{
    if (shouldRegister)
    {
        EventsManager::Singleton()->RegisterListener(this);
    }
}

EventListener::EventListener(const Event::EventType& eventType)
{
    EventsManager::Singleton()->RegisterListener(this, eventType);
}

EventListener::EventListener(std::vector<Event::EventType> eventTypes)
{
    for (Event::EventType& eventType : eventTypes)
    {
        EventsManager::Singleton()->RegisterListener(this, eventType);
    }
}
