#include "Events/EventManager.h"

//-----------------------------------------------------------------------------
// EventManager Implementation
//-----------------------------------------------------------------------------

EventManager::EventManager()
{

}

EventManager::~EventManager()
{

}

void EventManager::AddListener(const std::function<void(const IEventPtr&)>& func, uint32_t eventType)
{
    m_eventListeners[eventType].push_back(func);
}

void EventManager::RemoveListener(const std::function<void(const IEventPtr&)>& func, uint32_t eventType)
{

}

void EventManager::TriggerEvent(const IEventPtr& event)
{
    for (uint32_t i = 0; i < m_eventListeners[event->GetType()].size(); i++)
    {
        m_eventListeners[event->GetType()][i](event);
    }
}