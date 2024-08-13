#pragma once

#include <cstddef>
#include <cstdint>

#include "EventManager.h"

//-----------------------------------------------------------------------------
// class Event_CloseWindow
//-----------------------------------------------------------------------------

class Event_CloseWindow : public Event
{
public:
    static const EventType s_type = 0xa21b37ee;

    Event_CloseWindow() {}
    virtual EventType GetType() const { return s_type; }
};

//-----------------------------------------------------------------------------
// class Event_BaseKeyEvent
//-----------------------------------------------------------------------------

class Event_KeyInterface : public Event
{
public:
    Event_KeyInterface(int keyId, int modifierFlags) {}

    int GetKeyId() const { return m_keyId; }
    int GetModifierFlags() const { return m_modifierFlags; }

private:
    int     m_keyId;
    int     m_modifierFlags;
};

//-----------------------------------------------------------------------------
// class Event_KeyDown
//-----------------------------------------------------------------------------

class Event_KeyDown : public Event_KeyInterface
{
public:
    static const EventType s_type = 0x6532d3a4;

    Event_KeyDown(int keyId, int modifierFlags) : Event_KeyInterface(keyId, modifierFlags) {}
    virtual EventType GetType() const { return s_type; }
};

//-----------------------------------------------------------------------------
// class Event_KeyUp
//-----------------------------------------------------------------------------

class Event_KeyUp : public Event_KeyInterface
{
public:
    static const EventType s_type = 0x0b93029b;

    Event_KeyUp(int keyId, int modifierFlags) : Event_KeyInterface(keyId, modifierFlags) {}
    virtual EventType GetType() const { return s_type; }
};