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

void RenderManager::Update(float timeStep)
{
    for (Scene* scene : m_scenes)
        scene->Update(timeStep);
}

void RenderManager::UpdateImgui(ImGuiIO& io, float timeStep)
{
    for (Scene* scene : m_scenes)
        scene->UpdateImgui(io, timeStep);
}