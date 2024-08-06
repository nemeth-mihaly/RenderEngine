#pragma once

#include <stddef.h>
#include <stdint.h>

#include "Events/EventManager.h"

//-----------------------------------------------------------------------------
// class BrushRadiusChangedEvent
//-----------------------------------------------------------------------------

class BrushRadiusChangedEvent : public IEvent
{
 public:
    static const uint32_t g_type;

    BrushRadiusChangedEvent(float vradius)
    {
        radius = vradius; 
    }

    virtual uint32_t GetType() const { return g_type; }

    float radius;
};