#pragma once

#include <memory>
#include <vector>

#include "EventManager.h"

#include "World.h"

#include "Actor.h"
#include "Components.h"

//-----------------------------------------------------------------------------
// class Renderer
//-----------------------------------------------------------------------------

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Init();
    void Draw();

    void OnCreateMeshDrawComponent(std::shared_ptr<IEvent> event);
    void OnDestroyMeshDrawComponent(std::shared_ptr<IEvent> event);

private:
    uint32_t        m_ShaderId;

    World           m_World;
    std::vector<std::shared_ptr<MeshDrawComponent>> m_MeshDrawComponents;
};