#pragma once

#include "interface/events/Event.h"

#include <vector>

class KeyEvent;
class CharEvent;
class MouseButtonEvent;

class EventListener
{
public:

    EventListener();
    EventListener(const bool& shouldRegister);
    EventListener(const Event::EventType& eventType);
    EventListener(std::vector<Event::EventType> eventTypes);

    virtual void HandleEvent(KeyEvent* keyEvent) {};
    virtual void HandleEvent(CharEvent* charEvent) {};
    virtual void HandleEvent(MouseButtonEvent* mouseButtonEvent) {};
};

