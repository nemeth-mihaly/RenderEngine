#include "EventManager.h"

//-----------------------------------------------------------------------------
// EventManager Implementation
//-----------------------------------------------------------------------------

void EventManager::AddListener(EventType type, const std::function<void(std::shared_ptr<IEvent>)>& function)
{
    m_Listeners[type].push_back(function);
}

void EventManager::TriggerEvent(const std::shared_ptr<IEvent>& event)
{
    auto findItr = m_Listeners.find(event->GetType());
    if (findItr != m_Listeners.end())
    {
        const std::vector<std::function<void (std::shared_ptr<IEvent>)>>& listeners = findItr->second;
        for (auto itr = listeners.begin(); itr != listeners.end(); itr++)
        {
            const std::function<void (std::shared_ptr<IEvent>)>& function = (*itr);
            function(event);
        }
    }
}