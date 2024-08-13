#include "EventManager.h"

#include <cstdio>

//-----------------------------------------------------------------------------
// EventManager Implementation
//-----------------------------------------------------------------------------

void EventManager::AddListener(const std::function<void(std::shared_ptr<Event>)>& fn, EventType type) 
{
    // printf_s("Attempting to add listener function for event type (%u).\n", type);

    m_eventListeners[type].push_back(fn);
}

// void EventManager::RemoveListener(const std::function<void(std::shared_ptr<Event>)>& fn, EventType type) {}

void EventManager::TriggerEvent(const std::shared_ptr<Event> event) 
{
    // printf_s("Attempting to trigger event type (%u).\n", event->GetType());

    auto findItr = m_eventListeners.find(event->GetType());
    if (findItr != m_eventListeners.end())
    {
        std::vector<std::function<void(std::shared_ptr<Event>)>> listeners = findItr->second;
        for (auto itr = listeners.begin(); itr != listeners.end(); itr++)
        {
            (*itr)(event);
        }
    }
}