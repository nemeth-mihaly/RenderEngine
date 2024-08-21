#include "Console.h"

#include "3rdParty/ImGui/imgui.h"
#include "3rdParty/ImGui/imgui_impl_glfw.h"
#include "3rdParty/ImGui/imgui_impl_opengl3.h"

#include "Application.h"

//-----------------------------------------------------------------------------
// Console Implementation
//-----------------------------------------------------------------------------

Console::Console()
{

}

void Console::Initialize()
{

}

void Console::Draw()
{
    ImGui::Begin("Console");

    float height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("Console.ScrollingRegion", ImVec2(0, -height), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar);

    for (auto itr = m_LastTexts.begin(); itr != m_LastTexts.end(); itr++)
    {
        ImGui::TextUnformatted((*itr).c_str());
    }
    
    if (m_bScroll) 
    {
        ImGui::SetScrollHereY(1.0f);
        m_bScroll = false;
    }
    
    ImGui::EndChild();

    ImGui::Separator();

    char buf[128];
    memset(buf, 0, sizeof(buf));

    bool bReclaimfocus = false;
    if (ImGui::InputText("Console.InputText", buf, 128, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll))
    {
        std::string s(buf);
        m_LastTexts.push_back(s);

        std::vector<std::string> tokens;

        int pos = 0;
        while (pos < s.size())
        {
            pos = s.find(' ');
            std::string token = s.substr(0, pos);
            tokens.push_back(token);
            s.erase(0, pos + 1);
        }

        tokens.push_back(s);

        if (stricmp(tokens[0].c_str(), "HELP") == 0)
        {   
            
        }
        else if (stricmp(tokens[0].c_str(), "CLEAR") == 0)
        {
            m_LastTexts.clear();
        }
        else if (stricmp(tokens[0].c_str(), "QUIT") == 0)
        {
            g_pApp->GetEventManager().TriggerEvent(std::make_shared<AppQuitEvent>());
        }
        else if (stricmp(tokens[0].c_str(), "CREATEACTOR") == 0)
        {
            g_pApp->CreateActor();
        }
        else if (stricmp(tokens[0].c_str(), "DESTROYACTOR") == 0)
        {
            ActorId actorId = (ActorId)std::stoi(tokens[1]);
            g_pApp->DestroyActor(actorId);
        }
        else if (stricmp(tokens[0].c_str(), "MOVEACTOR") == 0)
        {
            ActorId actorId = (ActorId)std::stoi(tokens[1]);
            if (std::shared_ptr<Actor> actor = g_pApp->GetActor(actorId).lock())
            {
                glm::vec3 pos;
                pos.x = std::stof(tokens[2]);
                pos.y = std::stof(tokens[3]);
                pos.z = std::stof(tokens[4]);

                actor->SetPosition(pos);
            }
        }
        else
        {
            m_LastTexts.push_back("Unknown command '" + s + "'");
        }

        m_bScroll = true;
        bReclaimfocus = true;
    }

    ImGui::SetItemDefaultFocus();

    if (bReclaimfocus)
    {
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
    }

    ImGui::End();
}