#pragma once

#include <cstddef>
#include <cstdint>

typedef uint32_t ActorId;

//-----------------------------------------------------------------------------
// class Actor
//-----------------------------------------------------------------------------

class Actor
{
public:
    Actor(ActorId id);
    ~Actor();

    ActorId GetId() const { return m_id; }

private:
    ActorId m_id;
};