#pragma once

#include <cstdint>
#include <memory>
#include <functional>
#include <unordered_map>

typedef uint32_t EventType;

//-----------------------------------------------------------------------------
// class IEvent
//-----------------------------------------------------------------------------

class IEvent
{
public:
    virtual ~IEvent() = default;
    virtual EventType GetType() const = 0;
};

//-----------------------------------------------------------------------------
// class AppQuitEvent
//-----------------------------------------------------------------------------

class AppQuitEvent : public IEvent
{
public:
    static constexpr EventType s_Type = 0x636F39F2;

    AppQuitEvent() {}
    virtual EventType GetType() const { return s_Type; }
};

//-----------------------------------------------------------------------------
// class MouseMoveEvent
//-----------------------------------------------------------------------------

class MouseMoveEvent : public IEvent
{
public:
    static constexpr EventType s_Type = 0x7CD4DD93;

    MouseMoveEvent(float x, float y) : m_X(x), m_Y(y) {}
    virtual EventType GetType() const { return s_Type; }

    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }

private:
    float m_X;
    float m_Y;
};

//-----------------------------------------------------------------------------
// class BaseMouseButtonEvent
//-----------------------------------------------------------------------------

class BaseMouseButtonEvent : public IEvent
{
public:
    BaseMouseButtonEvent(int buttonId) : m_ButtonId(buttonId) {}

    int GetButtonId() const { return m_ButtonId; }

private:
    int m_ButtonId;
};

//-----------------------------------------------------------------------------
// class MouseButtonDownEvent
//-----------------------------------------------------------------------------

class MouseButtonDownEvent : public BaseMouseButtonEvent
{
public:
    static constexpr EventType s_Type = 0x3066B9AF;

    MouseButtonDownEvent(int buttonId) : BaseMouseButtonEvent(buttonId) {}
    virtual EventType GetType() const { return s_Type; }
};

//-----------------------------------------------------------------------------
// class MouseButtonUpEvent
//-----------------------------------------------------------------------------

class MouseButtonUpEvent : public BaseMouseButtonEvent
{
public:
    static constexpr EventType s_Type = 0x41CE3032;

    MouseButtonUpEvent(int buttonId) : BaseMouseButtonEvent(buttonId) {}
    virtual EventType GetType() const { return s_Type; }
};

//-----------------------------------------------------------------------------
// class BaseKeyEvent
//-----------------------------------------------------------------------------

class BaseKeyEvent : public IEvent
{
public:
    BaseKeyEvent(int keyId) : m_KeyId(keyId) {}

    int GetKeyId() const { return m_KeyId; }

private:
    int m_KeyId;
};

//-----------------------------------------------------------------------------
// class KeyDownEvent
//-----------------------------------------------------------------------------

class KeyDownEvent : public BaseKeyEvent
{
public:
    static constexpr EventType s_Type = 0xABE40AFE;

    KeyDownEvent(int keyId) : BaseKeyEvent(keyId) {}
    virtual EventType GetType() const { return s_Type; }
};

//-----------------------------------------------------------------------------
// class KeyUpEvent
//-----------------------------------------------------------------------------

class KeyUpEvent : public BaseKeyEvent
{
public:
    static constexpr EventType s_Type = 0xCCC6D316;

    KeyUpEvent(int keyId) : BaseKeyEvent(keyId) {}
    virtual EventType GetType() const { return s_Type; }
};

//-----------------------------------------------------------------------------
// class CreateMeshDrawComponentEvent
//-----------------------------------------------------------------------------

class CreateMeshDrawComponentEvent : public IEvent
{
public:
    static constexpr EventType s_Type = 0x2EB336C4;

    CreateMeshDrawComponentEvent(uint32_t actorId) : m_ActorId(actorId) {}
    virtual EventType GetType() const { return s_Type; }

    uint32_t GetActorId() const { return m_ActorId; }

private:
    uint32_t m_ActorId;
};

//-----------------------------------------------------------------------------
// class DestroyMeshDrawComponentEvent
//-----------------------------------------------------------------------------

class DestroyMeshDrawComponentEvent : public IEvent
{
public:
    static constexpr EventType s_Type = 0x0153EEF3;

    DestroyMeshDrawComponentEvent(uint32_t actorId) : m_ActorId(actorId) {}
    virtual EventType GetType() const { return s_Type; }

    uint32_t GetActorId() const { return m_ActorId; }

private:
    uint32_t m_ActorId;
};

//-----------------------------------------------------------------------------
// class EventManager
//-----------------------------------------------------------------------------

class EventManager
{
public:
    void AddListener(EventType type, const std::function<void(std::shared_ptr<IEvent>)>& function);
    // void RemoveListener(EventType type, const std::function<void(std::shared_ptr<IEvent>)>& function) {}

    void TriggerEvent(const std::shared_ptr<IEvent>& event);

private:
    std::unordered_map<EventType, std::vector<std::function<void(std::shared_ptr<IEvent>)>>> m_Listeners;
};