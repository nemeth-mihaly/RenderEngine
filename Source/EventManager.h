#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>

typedef uint32_t EventType;

#define BIND_EVENT_FN(fn, ptr) std::bind(fn, ptr, std::placeholders::_1)

//-----------------------------------------------------------------------------
// class Event
//-----------------------------------------------------------------------------

class Event
{
public: 
    virtual ~Event() { /* printf_s("~Event()\n"); */ }
    virtual EventType GetType() const = 0;
};

//-----------------------------------------------------------------------------
// class EventManager
//-----------------------------------------------------------------------------

class EventManager
{
public:
    void AddListener(const std::function<void(std::shared_ptr<Event>)>& fn, EventType type);
    // void RemoveListener(const std::function<void(std::shared_ptr<Event>)>& fn, EventType type);

    void TriggerEvent(const std::shared_ptr<Event> event);

private:
    std::unordered_map<uint32_t, std::vector<std::function<void(std::shared_ptr<Event>)>>> m_eventListeners;
};