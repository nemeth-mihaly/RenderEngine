#include "Renderer.h"

#include <cstdio>
#include <fstream>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "Application.h"

//-----------------------------------------------------------------------------
// Renderer Implementation
//-----------------------------------------------------------------------------

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
    glDeleteShader(m_ShaderId);
}

void Renderer::Init()
{
    uint32_t vsId, fsId;
    m_ShaderId = glCreateProgram();

    {
        std::ifstream file("Assets/Shaders/Actor.vert", std::ios::binary | std::ios::ate);
        size_t size = (size_t)file.tellg();
        file.seekg(0);

        char* pSource = new char[size + 1];
        file.read(pSource, size);
        pSource[size] = '\0';
        file.close();

        vsId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vsId, 1, &pSource, nullptr);
        glCompileShader(vsId);

        delete[] pSource;
    }

    {
        std::ifstream file("Assets/Shaders/Actor.frag", std::ios::binary | std::ios::ate);
        size_t size = (size_t)file.tellg();
        file.seekg(0);

        char* pSource = new char[size + 1];
        file.read(pSource, size);
        pSource[size] = '\0';
        file.close();

        fsId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsId, 1, &pSource, nullptr);
        glCompileShader(fsId);

        delete[] pSource;
    }

    glAttachShader(m_ShaderId, vsId);
    glAttachShader(m_ShaderId, fsId);
    glLinkProgram(m_ShaderId);

    glDeleteShader(vsId);
    glDeleteShader(fsId);

    glUniformBlockBinding(m_ShaderId, glGetUniformBlockIndex(m_ShaderId, "Matrices"), 0);

    m_World.Init();

    g_pApp->GetEventManager().AddListener(CreateMeshDrawComponentEvent::s_Type, std::bind(&Renderer::OnCreateMeshDrawComponent, this, std::placeholders::_1));
    g_pApp->GetEventManager().AddListener(DestroyMeshDrawComponentEvent::s_Type, std::bind(&Renderer::OnDestroyMeshDrawComponent, this, std::placeholders::_1));
}

void Renderer::Draw()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_World.Draw();

    glUseProgram(m_ShaderId);

    for (auto itr = m_MeshDrawComponents.begin(); itr != m_MeshDrawComponents.end(); itr++)
    {
        if (auto actor = (*itr)->GetOwner().lock())
        {
            if (auto transformComponent = actor->GetComponent<TransformComponent>().lock())
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), transformComponent->GetPosition());
                glUniformMatrix4fv(glGetUniformLocation(m_ShaderId, "uModel"), 1, GL_FALSE, glm::value_ptr(model));  
            }

            (*itr)->GetMesh()->Draw();
        }
    }

    glUseProgram(0);
}

void Renderer::OnCreateMeshDrawComponent(std::shared_ptr<IEvent> event)
{
    auto evt = std::static_pointer_cast<CreateMeshDrawComponentEvent>(event);
    
    ActorId actorId = (ActorId)evt->GetActorId();
    if (std::shared_ptr<Actor> actor = g_pApp->GetActor(actorId).lock())
    {
        if (std::shared_ptr<MeshDrawComponent> meshDrawComponent = actor->GetComponent<MeshDrawComponent>().lock())
        {
            m_MeshDrawComponents.push_back(meshDrawComponent);
        }
    }
}

void Renderer::OnDestroyMeshDrawComponent(std::shared_ptr<IEvent> event)
{
    auto evt = std::static_pointer_cast<DestroyMeshDrawComponentEvent>(event);

    ActorId actorId = (ActorId)evt->GetActorId();
    for (auto itr = m_MeshDrawComponents.begin(); itr != m_MeshDrawComponents.end(); itr++)
    {
        if (auto actor = (*itr)->GetOwner().lock())
        {
            if (actor->GetId() != actorId) { continue; }

            if (m_MeshDrawComponents.size() > 1) 
            { 
                std::swap(m_MeshDrawComponents.back(), (*itr)); 
            }
            
            m_MeshDrawComponents.pop_back();
            break;
        }
    }
}