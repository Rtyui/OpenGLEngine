#pragma once

#include <string>

class EventListener;

class Event
{
public:

    enum class EventType
    {
        KeyEvent,
        CharEvent,
        MouseButtonEvent,
        WindowSizeEvent
    };

protected:

public:

    virtual EventType GetType() = 0;

    virtual void GetHandled(EventListener* eventListener) = 0;

    static std::string EventTypeToString(const EventType& eventType);
protected:

    Event() = default;
};

