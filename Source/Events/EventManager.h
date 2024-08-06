#pragma once

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>

enum EventType
{
    EventType_BrushRadiusChanged,
};

//-----------------------------------------------------------------------------
// class Event
//-----------------------------------------------------------------------------

class IEvent
{
 public:
    virtual ~IEvent() = default;

    virtual uint32_t GetType() const = 0;
};

typedef std::shared_ptr<IEvent> IEventPtr;

//-----------------------------------------------------------------------------
// class BrushRadiusChangedEvent
//-----------------------------------------------------------------------------

// class BrushRadiusChangedEvent : public IEvent
// {
//  public:
//     static uint32_t g_type;
// 
//     BrushRadiusChangedEvent(float vradius)
//     {
//         radius = vradius;
//     }
// 
//     virtual uint32_t GetType() const { return g_type; }
// 
//     float radius;
// };

//-----------------------------------------------------------------------------
// class EventManager
//-----------------------------------------------------------------------------

class EventManager
{
 public:
    EventManager();
    ~EventManager();

    void AddListener(const std::function<void(const IEventPtr&)>& func, uint32_t eventType);
    void RemoveListener(const std::function<void(const IEventPtr&)>& func, uint32_t eventType);

    void TriggerEvent(const IEventPtr& event);

 private:
    std::unordered_map<uint32_t, std::vector<std::function<void(const IEventPtr&)>>> m_eventListeners;
};