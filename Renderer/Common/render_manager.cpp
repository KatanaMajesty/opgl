#include "Common/render_manager.h"

RenderManager::RenderManager(GLFWwindow* context)
    : m_context(context)
{
}

RenderManager::~RenderManager()
{
    for (Scene* scene : m_scenes)
        delete scene;
}
    
void RenderManager::SetCurrentScene(Scene* scene) 
{ 
    if (m_currentScene == scene || !scene)
        return;

    if (m_currentScene)
        m_currentScene->OnDetatch();

    m_currentScene = scene;
    m_currentScene->OnAttach();
}

void RenderManager::Update(float timeStep)
{
    // we do not render anything, if no current scene selected
    if (!m_currentScene)
        return;

    m_currentScene->Update(timeStep);
}

void RenderManager::UpdateImgui(ImGuiIO& io, float timeStep)
{
    // if no current scene selected, we render our own ImGui menu
    if (!m_menuEnabled)
    {
        ImGui::Begin("Render Manager");

        ImGui::Text("Select Scene to be rendered");
        if (ImGui::BeginTable("Scene List", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
        {
            ImGui::TableHeadersRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("Scene Name");
            ImGui::TableNextRow();
            for (size_t i = 0; i < m_scenes.size(); i++)
            {
                Scene* scene = m_scenes[i];
                ImGui::TableSetColumnIndex(0);
                if (ImGui::Selectable(scene->GetName().c_str(), scene == m_currentScene))
                {
                    std::cout << "Selected scene " << scene->GetName() << std::endl;
                    SetCurrentScene(scene);
                }
                ImGui::TableNextRow();
            }
            ImGui::EndTable();
        }

        ImGui::End();
        return;   
    }

    if (m_currentScene)    
        m_currentScene->UpdateImgui(io, timeStep);
}