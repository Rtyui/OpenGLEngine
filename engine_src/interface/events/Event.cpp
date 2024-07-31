#include "interface/events/Event.h"

std::string Event::EventTypeToString(const EventType& eventType)
{
    switch (eventType)
    {
        case EventType::KeyEvent:
        {
            return "KeyEvent";
            break;
        }
        default:
        {
            return "Unknown";
        }
    }
}
