#pragma once

#include "interface/events/EventListener.h"
#include "interface/events/Event.h"

#include <vector>
#include <map>

class EventsManager
{
public:

    std::vector<EventListener*> m_RegisteredListeners;
    std::map<Event::EventType, std::vector<EventListener*>> m_SpecificListeners;

private:

    static EventsManager* s_EventsManager;

public:

    static EventsManager* Singleton();
    static void Destroy();

    void RegisterListener(EventListener* eventListener);
    void RegisterListener(EventListener* eventListener, Event::EventType eventType);
    void RegisterEvent(Event* event);


};

