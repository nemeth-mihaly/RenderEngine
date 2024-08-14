#include "Actor.h"

#include <cstdio>

//-----------------------------------------------------------------------------
// Actor Implementation
//-----------------------------------------------------------------------------

Actor::Actor(ActorId id)
{
    m_id = id;

    printf("Creating Actor (%u)\n", GetId());
}

Actor::~Actor()
{
    printf("Destroying Actor (%u)\n", GetId());
}