#include "UI/EditorUI.h"

#include "3rdParty/ImGui/imgui.h"
#include "3rdParty/ImGui/imgui_impl_glfw.h"
#include "3rdParty/ImGui/imgui_impl_opengl3.h"

#include "Application.h"

//-----------------------------------------------------------------------------
// EditorUI Implementation
//-----------------------------------------------------------------------------

EditorUI::EditorUI()
{
    m_bShowSceneObjectList = false;
}

EditorUI::~EditorUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EditorUI::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight(&ImGui::GetStyle());
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/consola.ttf", 14);

    ImGui_ImplGlfw_InitForOpenGL(&g_pApp->GetWindow(), true);
    ImGui_ImplOpenGL3_Init();
}

void EditorUI::Draw()
{
    ImGui::ShowDemoWindow();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::MenuItem("SceneObjects", nullptr, &m_bShowSceneObjectList)) {}

        ImGui::EndMainMenuBar();
    }

    if (m_bShowSceneObjectList)
    {
        ImGui::Begin("SceneObject List", &m_bShowSceneObjectList);

        

        ImGui::End();
    }
}
