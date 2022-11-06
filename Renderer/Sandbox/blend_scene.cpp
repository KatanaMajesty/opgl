#include "Sandbox/blend_scene.h"

#include "Common/window.h"

BlendScene::BlendScene(GLFWwindow* context) : Scene(context, "Blending Scene")
    , m_camera(context) 
{
    Window::GetInstance()->AddCursorCallback([&](Window* window, double x, double y) { m_camera.UpdateEulers(x, y, window->IsCursorVisible() || !window->IsFocused()); });
    Window::GetInstance()->AddScrollCallback([&](Window* window, double y) { m_camera.UpdateScroll((float) y); });
    m_depthShader = CreateShader("../Data/Shader/depth_shader.vert", "../Data/Shader/depth_shader.frag");
}

void BlendScene::Update(float timeStep)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    model = glm::translate(model, glm::vec3(0.0f));

    m_camera.UpdatePosition(timeStep);

    int32_t width = Window::GetInstance()->GetWidth();
    int32_t height = Window::GetInstance()->GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(m_camera.Fov()), (float)width/(float)height, 0.1f, 100.0f);

    Shader::Bind(*m_depthShader);
    m_depthShader->SetUniformMat4("u_model", model);
    m_depthShader->SetUniformMat4("u_projection", projection);
    m_depthShader->SetUniformMat4("u_view", m_camera.GetLookAtMatrix());
}

void BlendScene::UpdateImgui(ImGuiIO& io, float timeStep)
{

}